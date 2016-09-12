#include "boost/program_options.hpp" 

#include <iostream> 
#include <string> 
#include "grid.h"

using std::string;
using std::cout;
using std::cin;
using std::endl;

namespace po = boost::program_options;

namespace 
{ 
    const size_t ERROR_IN_COMMAND_LINE = 1; 
    const size_t SUCCESS = 0; 
    const size_t ERROR_UNHANDLED_EXCEPTION = 2; 

} // namespace 

/*!
 * Show the help message with parameter options.
*/
size_t showHelp(po::options_description desc) 
{
    std::cout << "Hotspot detection parameters:" << std::endl 
            << desc << std::endl; 
    return SUCCESS;
}


int main(int argc, char** argv) 
{ 
    try 
    { 
        /** Define and parse the program options 
        */ 
         
        po::options_description desc("Options"); 
        desc.add_options() 
            ("help", "Print help messages") 
            ("trace", po::value<string>(), "The trace file") 
            ("square,s", po::value<int>(), "The width of the square partitions considered by the algorithm")
            ("width", po::value<int>(), "The width of the simulated area. If not explicit informed, it will consider the values in the trace file")
            ("heigh", po::value<int>(), "The height of the simulated area. If not explicit informed, it will consider the values in the trace file")
            ("twindow,t", po::value<int>(), "The time window in seconds for computing the dropoff.")
            ("relays,r", po::value<int>()->default_value(10), "How many relay points you want to add"); 

        po::positional_options_description p;
        p.add("trace", -1);
        po::variables_map vm; 
        try 
        { 
            po::store(po::command_line_parser(argc, argv).options(desc).positional(p).run(),  
            vm); // can throw 

            /** --help option 
            */ 
            if (vm.count("help")) 
            { 
                return showHelp(desc);
            } 

            po::notify(vm); // throws on error, so do after help in case 
            // there are any problems
        } 
        catch(po::error& e) 
        { 
            std::cerr << "ERROR: " << e.what() << std::endl << std::endl; 
            std::cerr << desc << std::endl; 
            return ERROR_IN_COMMAND_LINE; 
        } 

        if (vm.count("trace") != 1) {
            cout << "You need to pass the trace file path" << endl;
            return showHelp(desc);
        }
        if (!vm.count("square")) {
            cout << "You need to pass the partitions square size" << endl;
            return showHelp(desc);
        }
        if (!vm.count("twindow")) {
            cout << "You need to pass the time window" << endl;
            return showHelp(desc);
        }


        hd::Grid grid(vm["trace"].as<string>(), vm["square"].as<int>(), vm["twindow"].as<int>(), vm["relays"].as<int>());

        if (vm.count("width")) {
            grid.setWidth(vm["width"].as<int>());
        }
        if (vm.count("height")) {
            grid.setHeight(vm["height"].as<int>());
        }

        grid.detectHotspots();
        
        std::string temp;
        getline(cin, temp);

    } 
    catch(std::exception& e) 
    { 
        std::cerr << "Unhandled Exception reached the top of main: " 
        << e.what() << ", application will now exit" << std::endl; 
        return ERROR_UNHANDLED_EXCEPTION; 

    } 

    return SUCCESS; 

} // main 