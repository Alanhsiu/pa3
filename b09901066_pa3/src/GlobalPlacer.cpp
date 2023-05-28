#include "GlobalPlacer.h"
#include "ExampleFunction.h"
#include "NumericalOptimizer.h"

GlobalPlacer::GlobalPlacer(Placement& placement)
    : _placement(placement) {
}

void GlobalPlacer::place() {
    ///////////////////////////////////////////////////////////////////
    // The following example is only for analytical methods.
    // if you use other methods, you can skip and delete it directly.
    //////////////////////////////////////////////////////////////////
    ExampleFunction ef;  // require to define the object function and gradient function

    double chip_width = _placement.boundryRight() - _placement.boundryLeft();
    double chip_height = _placement.boundryTop() - _placement.boundryBottom();
    double left_bound = _placement.boundryLeft();
    double bottom_bound = _placement.boundryBottom();

    int num_blocks = _placement.numModules();
    vector<double> x(num_blocks * 2);

    //////////// Cluster Start ////////////
    set<int> _check;
    vector<int> _order;  // the order of placeing the modules
    vector<Net> nets = _placement.getNets();

    // sort the nets by the number of pins
    sort(nets.begin(), nets.end(), [](Net& a, Net& b) {
        return a.numPins() > b.numPins();
    });

    for (int i = 0; i < nets.size(); i++) {
        int numPins = nets[i].numPins();
        for (int j = 0; j < numPins; j++) {
            // go through each module in the net
            int moduleId = nets[i].pin(j).moduleId();
            // check if the id is already in _check set
            if (_check.find(moduleId) == _check.end()) {
                _check.insert(moduleId);
                _order.push_back(moduleId);
            }
        }
    }
    //////////// Cluster End ////////////

    //////////// Random Start ////////////
    // std::uniform_real_distribution<double> distribution(0.0, 1.0);
    // int max_num_section = sqrt(num_blocks);

    // for (int b = max_num_section/2; b < max_num_section; b++) 
    // {
    //     // divide the chip into bins
    //     int num_section = param.best_num_section;
    //     int binWidth = chip_width / num_section;
    //     int binHeight = chip_height / num_section;
    //     int binSize = num_blocks / (num_section * num_section);

    //     // generate random coordinates for each bin
    //     for (int s = 1; s < 10000; s++) {
    //         std::default_random_engine generator(s);
    //         vector<double> sortX;
    //         vector<double> sortY;
    //         for (int i = 0; i < num_section; ++i) {
    //             int idx_x = (i + num_section / 2) % num_section;
    //             for (int j = 0; j < num_section; ++j) {
    //                 for (int k = 0; k < binSize; ++k) {
    //                     double randomX = left_bound + binWidth * idx_x + binWidth * distribution(generator);
    //                     double randomY = bottom_bound + binHeight * j + binHeight * distribution(generator);
    //                     sortX.push_back(randomX);
    //                     sortY.push_back(randomY);
    //                 }
    //             }
    //         }
    //         for (int i = 0; i < num_blocks; i++) {
    //             x[_order[i] * 2] = sortX[i];
    //             x[_order[i] * 2 + 1] = sortY[i];
    //         }

    //         NumericalOptimizer no(ef);
    //         no.setX(x);
    //         for (int i = 0; i < num_blocks; i++)
    //             _placement.module(i).setPosition(no.x(i * 2), no.x(i * 2 + 1));

    //         if (_placement.computeHpwl() < param.best_HPWL) {
    //             param.best_num_section = num_section;
    //             param.best_HPWL = _placement.computeHpwl();
    //             param.best_seed = s;
    //             cout << "best_num_section: " << param.best_num_section << endl;
    //             cout << "best seed: " << param.best_seed << ", best HPWL: " << param.best_HPWL << endl;
    //         }
    //     }
    // }

    //////////// Random End ////////////

    std::uniform_real_distribution<double> distribution(0.0, 1.0);
    std::default_random_engine generator(param.best_seed);
    int binWidth = chip_width / param.best_num_section;
    int binHeight = chip_height / param.best_num_section;
    int binSize = num_blocks / (param.best_num_section * param.best_num_section);
    vector<double> sortX;
    vector<double> sortY;
    for (int i = 0; i < param.best_num_section; ++i) {
        int idx_x = (i + param.best_num_section / 2) % param.best_num_section;
        for (int j = 0; j < param.best_num_section; ++j) {
            for (int k = 0; k < binSize; ++k) {
                double randomX = left_bound + binWidth * idx_x + binWidth * distribution(generator);
                double randomY = bottom_bound + binHeight * j + binHeight * distribution(generator);
                sortX.push_back(randomX);
                sortY.push_back(randomY);
            }
        }
    }
    // assign the coordinates to each module
    for (int i = 0; i < num_blocks; i++) {
        x[_order[i] * 2] = sortX[i];
        x[_order[i] * 2 + 1] = sortY[i];
    }

    NumericalOptimizer no(ef);
    no.setX(x);  // set initial solution

    for (int i = 0; i < num_blocks; i++) 
        _placement.module(i).setPosition(no.x(i * 2), no.x(i * 2 + 1));
    
    // cout << "Objective: " << no.objective() << endl;
    ////////////////////////////////////////////////////////////////
}

void GlobalPlacer::plotPlacementResult(const string outfilename, bool isPrompt) {
    ofstream outfile(outfilename.c_str(), ios::out);
    outfile << " " << endl;
    outfile << "set title \"wirelength = " << _placement.computeHpwl() << "\"" << endl;
    outfile << "set size ratio 1" << endl;
    outfile << "set nokey" << endl
            << endl;
    outfile << "plot[:][:] '-' w l lt 3 lw 2, '-' w l lt 1" << endl
            << endl;
    outfile << "# bounding box" << endl;
    plotBoxPLT(outfile, _placement.boundryLeft(), _placement.boundryBottom(), _placement.boundryRight(), _placement.boundryTop());
    outfile << "EOF" << endl;
    outfile << "# modules" << endl
            << "0.00, 0.00" << endl
            << endl;
    for (size_t i = 0; i < _placement.numModules(); ++i) {
        Module& module = _placement.module(i);
        plotBoxPLT(outfile, module.x(), module.y(), module.x() + module.width(), module.y() + module.height());
    }
    outfile << "EOF" << endl;
    outfile << "pause -1 'Press any key to close.'" << endl;
    outfile.close();

    if (isPrompt) {
        char cmd[200];
        sprintf(cmd, "gnuplot %s", outfilename.c_str());
        if (!system(cmd)) {
            cout << "Fail to execute: \"" << cmd << "\"." << endl;
        }
    }
}

void GlobalPlacer::plotBoxPLT(ofstream& stream, double x1, double y1, double x2, double y2) {
    stream << x1 << ", " << y1 << endl
           << x2 << ", " << y1 << endl
           << x2 << ", " << y2 << endl
           << x1 << ", " << y2 << endl
           << x1 << ", " << y1 << endl
           << endl;
}
