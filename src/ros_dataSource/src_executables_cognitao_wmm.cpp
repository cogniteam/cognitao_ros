/**
 * @brief CogniTao's World Model monitor, prints all variables
 * 
 * @file cognitao_wmm.cpp
 * 
 * @author Igor Makhtes (igor@cogniteam.com)
 * @date 2020-03-04
 * @copyright Cogniteam (c) 2020
 *    
 * MIT License
 *   
 * Permission is hereby granted, free of charge, to any person obtaining a  copy
 * of this software and associated documentation files (the 'Software'), to deal
 * in the Software without restriction, including without  limitation the rights
 * to use, copy, modify, merge,  publish,  distribute,  sublicense,  and/or sell
 * copies of the Software, and  to  permit  persons  to  whom  the  Software  is 
 * furnished to do so, subject to the following conditions:
 *   
 * The   above   copyright   notice   and  this   permission   notice  shall  be
 * included in all copies or substantial portions of the Software.
 *   
 * THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY  KIND,  EXPRESS  OR
 * IMPLIED, INCLUDING BUT NOT LIMITED  TO  THE  WARRANTIES  OF  MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN  NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE  LIABLE  FOR  ANY  CLAIM,  DAMAGES  OR  OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 * 
 */


#include <chrono>
#include <thread>

#include <cognitao/CogniTao.h>
#include <cognitao/WorldModel.h>
#include <cognitao/data_sources/UdpMulticastDataSource.h>
#include "../include/cognitao_ros/ros_dataSource/RosDataSource.h"


using namespace std;


int main(int argc, char const *argv[]) {

    bool setMyId = true;

    // Generate UID
    srand(time(NULL));
    string myUniqueId = "wm_" + to_string(rand());

    if (argc > 1) {
        if (string(argv[1]) == "-a") {
            setMyId = false;
        }
    }

    WorldModel::setDataSource(RosDataSource::create());

    // Use global namespace
    WorldModel::setNamespace("/");

    cout << "WorldModel monitor v" << COGNITAO_VERSION << endl;
    cout << " - data_source = RosDataSource" << endl;
    cout << " - unique_id = /" << myUniqueId << endl;
    cout << " - namespace = /" << endl << endl;

    long long counter = 1;

    while (true) {

        if (setMyId) {
            // Identify myself to others
            WorldModel::setVar(myUniqueId, "alive");
        }

        cout << "WorldModel [" << myUniqueId << "] #" << (counter++) << endl;

        for (auto&& varName : WorldModel::getAllVariableNames()) {
            cout << " - " << varName << " = " << 
                    WorldModel::getVar(varName) << endl;
        }

        cout << "---" << endl;

        this_thread::sleep_for(chrono::seconds(1));
    }

    return 0;
}
