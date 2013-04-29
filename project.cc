//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include "project.h"
#include "ticket_m.h"

Define_Module(Project);

void Project::initialize()
{
    probabilityError = par("probError").doubleValue();
    queue = new cQueue("featurequeue");
}

void Project::handleMessage(cMessage *msg) {
    double randNumber = dblrand();
    //feature comes from entwickler and has no error
    int inEntwicklerGate = strcmp("inEntwickler",
            msg->getArrivalGate()->getName());

    if (inEntwicklerGate == 0){
        entwicklerHasFeature = false;
    }

    if (inEntwicklerGate == 0 && randNumber > probabilityError) {
        send(msg, "outKunde");
        if (!queue->isEmpty()){
            cMessage *secondmsg = (cMessage *)queue->pop();
            entwicklerHasFeature = true;
            send(secondmsg, "outEntwickler");
        }
    } else {
        queue->insert(msg);
        if (!entwicklerHasFeature) {
            msg = (cMessage *)queue->pop();
            entwicklerHasFeature = true;
            send(msg, "outEntwickler");
        }
    }

    cDisplayString& connDispStr = getDisplayString();
    if (queue->isEmpty()){
        connDispStr.parse("p=163,174;i=block/cogwheel,#00FF00");
    } else if (queue->getLength() < 10){
        connDispStr.parse("p=163,174;i=block/cogwheel,#FFFF00");
    } else {
        connDispStr.parse("p=163,174;i=block/cogwheel,#FF0000");
    }

}


