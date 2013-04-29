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

#include "kunde.h"
#include "ticket_m.h"

Define_Module(Kunde);

double probabilityRefuse;
int anzahlFeatures;
int timeStep = 3;

void Kunde::initialize() {
    Ticket *firstmsg = new Ticket("Feature");
    firstmsg->addPar("creationTime");
    firstmsg->par("creationTime").setDoubleValue(simTime().dbl());
    send(firstmsg, "out");

    Ticket *secondmsg = new Ticket("Feature");
    secondmsg->addPar("creationTime");
    secondmsg->par("creationTime").setDoubleValue((simTime() + timeStep).dbl());
    scheduleAt(simTime() + timeStep, secondmsg);

    probabilityRefuse = par("probRefuse").doubleValue();
    anzahlFeatures = par("anzahlFeatures").longValue() - 2;

}

void Kunde::handleMessage(cMessage *msg) {
    if (anzahlFeatures >= 0) {
        //endSimulation();

        if (msg->isSelfMessage()) {
            //neue Nachrichten in Zeitschritten erzeugen
            send(msg, "out");
            //ev << "Neues Feature verschickt ";
            //char numstr[21];
            //sprintf(numstr, "%d", age);
            //result = name + numstr;

            Ticket *newmsg = new Ticket(
                    "Feature " + (100 - anzahlFeatures));
            newmsg->setName("Feature ");
            newmsg->addPar("creationTime");
            newmsg->par("creationTime").setDoubleValue(
                    (simTime() + timeStep).dbl());
            scheduleAt(simTime() + timeStep, newmsg);
            anzahlFeatures -= 1;
        } else {
            //feature wurde bearbeitet
            //soll Feature nochmal ueberarbeitet werden?
            double randNumber = dblrand();
            if (randNumber < probabilityRefuse) {
                send(msg, "out");
            } else {
                //Anzahl verbleibender Features
                //ev << "n = " << anzahlFeatures << "\n";
                //komplette Bearbeitungszeit des features ausgeben
                long totaltime = simTime().dbl()
                        - msg->par("creationTime").doubleValue();
                ev << totaltime << " ";
                delete (msg);
                recordScalar("totaltime", totaltime);
            }
        }
    }
}
