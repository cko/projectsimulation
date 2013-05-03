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

package projektsimulation;

network Projektsimulation
{
    parameters:
        int numEntwickler = 4;
    @display("bgb=365,362");
    submodules:
        project: Project {
            @display("p=163,174;i=,#00ff00");
            gates:
                inEntwickler[numEntwickler];
                outEntwickler[numEntwickler];
        }
        entwickler[numEntwickler]: Entwickler {
            @display("p=163,310,m,10,80");
        }
        kunde: Kunde {
            @display("p=163,27");
        }
    connections:
        for i=0..numEntwickler-1 {
            project.outEntwickler[i] --> entwickler[i].in;
            entwickler[i].out --> project.inEntwickler[i];
        }
        
        kunde.out --> project.inKunde;
        project.outKunde --> kunde.in;
}
