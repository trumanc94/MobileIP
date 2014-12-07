#include <iostream>
#include <string>
#include <list>
#include <stdlib.h>
#include <thread>
#include <chrono>

using namespace std;

// Classes 
class mobileNode
{
   public:
      // Constructor
      mobileNode(string internetProtocol) { IP = internetProtocol; }
      
      // Member Functions
      string getIP() { return IP; }
   
   private:
      // Members
      string IP;   // This is the permanent IP of the MN's home address   
};

class homeAgent
{
   public:
      // Constructor
      homeAgent(string HA) { HAAddress = HA; }
      
      // Member Functions
      string getHA() { return HAAddress; }

      bool isEmpty() { return bindingTable.empty(); }
      
      void addEntry(string home, string coa, int time)
      {
         // Add new binding entry to Mobility Binding Table
         bindingEntry temp(home, coa, time);
         bindingTable.push_front(temp);
      }
      
      void printEntries()
      {
         // Print Binding Table title
         cout << "--------------------------------------------------";
         cout << endl;
         cout << "              Mobility Binding Table              ";
         cout << endl;
         cout << "--------------------------------------------------";
         cout << endl;
         cout << "| Home Address | Care-of-Address | Lifetime(sec) |";
         cout << endl;
         cout << "--------------------------------------------------";
         cout << endl;
                  
         // Iterate through Mobility Binding Table and print each entry
         std::list<bindingEntry>::const_iterator iterator;
         for(iterator = bindingTable.begin(); 
                              iterator != bindingTable.end(); ++iterator)
         {
             cout << " " << (*iterator).homeAddress << " | ";
             cout << (*iterator).COA << "   | ";
             cout << (*iterator).lifetime << endl;
         }
      }
      
   private:
      // Mobility Binding Table entries
      class bindingEntry
      {
         public:
            // Constructor
            bindingEntry(string home, string careOfAddress, int time)
                  :homeAddress(home), COA(careOfAddress), lifetime(time) {}
          
            // Members
            string homeAddress;  // Home address of a mobility node
            string COA;          // Care-of-Address of a mobility node
            int lifetime;        // Lifetime of the entry in seconds
      };
      
      // Members
      string HAAddress;                // Home Agent address
      list<bindingEntry> bindingTable; // Mobility Binding Table     
};

class foreignAgent
{
   public:
      // Constructor
      foreignAgent(string FA) { FAAddress = FA; }
         
      // Member Functions
      string getFA() { return FAAddress; }         
         
      void addEntry(string home, string HA, string MAC, int time)
      {
         // Add new binding entry to Mobility Binding Table
         visitorEntry temp(home, HA, MAC, time);
         visitorList.push_front(temp);
      }
      
      void printEntries()
      {
         // Print Binding Table title
         cout << "-------------------------------------------------------------------";
         cout << endl;
         cout << "                           Visitor List                            ";
         cout << endl;
         cout << "-------------------------------------------------------------------";
         cout << endl;
         cout << "| Home Address |  Home Agent  |   Media Address   | Lifetime(sec) |";
         cout << endl;
         cout << "|              |    Address   |                   |               |";
         cout << endl;
         cout << "-------------------------------------------------------------------";
         cout << endl;
                  
         // Iterate through Mobility Binding Table and print each entry
         std::list<visitorEntry>::const_iterator iterator;
         for(iterator = visitorList.begin(); 
                              iterator != visitorList.end(); ++iterator)
         {
             cout << " " << (*iterator).homeAddress << " | ";
             cout << (*iterator).HAAddress << " | ";
             cout << (*iterator).mediaAddress << " | ";
             cout << (*iterator).lifetime << endl;
         }
      }
               
   private:
      // Visitor List entries
      class visitorEntry
      {
         public:
            // Constructor
            visitorEntry(string home, string HA, string MAC, int time)
                  :homeAddress(home), HAAddress(HA), mediaAddress(MAC),
                                                          lifetime(time){}
                                                          
            // Members
            string homeAddress;  // Home address of a mobility node
            string HAAddress;    // Home agent address
            string mediaAddress; // MAC address
            int lifetime;        // Lifetime of the entry in seconds
      };
      
      // Members
      string FAAddress;               // Foreign Agent address
      list<visitorEntry> visitorList; // Visitor List           
};

class datagram
{
   public:
      // Constructor
      datagram(string foreign, string home, string mobile, int time, int i)
               :COA(foreign), HA(home), MA(mobile), lifetime(time), ID(i){}
               
      // Member Functions
      string getCOA() { return COA; }
      string getHA() { return HA; }
      string getMA() { return MA; }
      int getLifetime() { return lifetime; }

   private:
      // Members
      string COA;    // COA of foreign agent
      string HA;     // Home agent address
      string MA;     // Permanent mobile node address
      int lifetime;  // Lifetime of the registration entry
      int ID;        // Identification number of the datagram      
};

/*
class applicationLayer
{
   public:
      string message;

};

class transportLayer
{
   // public
   class UDPsegment
   {
      
   }
};

class networkLayer
{
   
};

struct registerData()
{
   string COA;
   string HAAddress;
   string MNAddress;
   int lifeTime
   int id;
};
*/

// Function Prototype Declarations
void Sleep(int);
string generateIP();
void registerMN(mobileNode, homeAgent, foreignAgent);

// Main Simulation
int main()
{
   srand((unsigned int) time(NULL));
   
   // Initialize objects
   mobileNode MN(generateIP());
   homeAgent HA(generateIP());
   foreignAgent FA(generateIP());
   
cout << "Mobile Node IP: " << MN.getIP() << endl;
cout << "Home Agent address: " << HA.getHA() << endl;
HA.printEntries();
cout << endl;
cout << "Foreign Agent address: " << FA.getFA() << endl;
FA.printEntries();
cout << endl;

   // Display Main Menu
   cout << "            Mobile IP            " << endl;
   cout << "---------------------------------" << endl;
/*
   cout << "1. Mobile Node in Home Network" << endl;
   cout << "2. Mobile Node in Foreign Network" << endl;
   cout << "Enter your selection: ";
   cin >> selection;
*/
  
   
   // Register Mobile Node to Host Agent
   registerMN( MN, HA, FA );   

   
   return 0;
}

// Function Implementation
void Sleep(int time) { this_thread::sleep_for(chrono::seconds(time)); }

string generateIP()
{
   // Generate random IP address
   int ipInt[4] = { rand() % 31 + 192,
      rand() % 254 + 1,
      rand() % 254 + 1,
      rand() % 254 + 1};
   string IP = to_string(ipInt[0]);   
   for(int i = 1; i < 4; i++) IP += "." + to_string(ipInt[i]);
   
   // return the IP
   return IP;
}

/* steps on page 568 */
void registerMN( mobileNode m, homeAgent h, foreignAgent f )
{
   // Listen for broadcast???
   cout << "Mobile Node listening for Home Agent or Foreign Agent advertisement..." << endl << endl;
   Sleep(2);
   
   // Check if function is not at home network
   if( h.isEmpty() )
   {    
      // Confirm Mobile Node is in foreign network
      cout << "Mobile Node is in foreign network!" << endl << endl;
      Sleep(2);
      
      // MN: send request to foreign agent
      cout << "Mobile Node: Sending registration request to Foreign Agent..." << endl << endl;
      Sleep(2);

      // FA: relay request to host agent
      cout << "Foreign Agent: Sending registration request to Host Agent..." << endl << endl;
      Sleep(2);
      
      // FA: update visitor list
      cout << "Foreign Agent: Updating Visitor List..." << endl << endl;
      Sleep(2);
      f.addEntry(m.getIP(), h.getHA(), "temporary MAC address", 20);
      f.printEntries();
      cout << endl << "Visitor List is updated!" << endl << endl;
      Sleep(3);
      
      // HA: send reply to foreign agent
      cout << "Home Agent: Sending registration reply to Foreign Agent..." << endl << endl;
      Sleep(2);
      
      // HA: update binding table
      cout << "Home Agent: Updated Mobile Binding Table..." << endl << endl;
      Sleep(2);
      h.addEntry(m.getIP(), f.getFA(), 30);
      h.printEntries();
      cout << endl << "Mobile Binding Table is updated!" << endl << endl;
      Sleep(3);
      
      // FA: relay reply to mobile node
      cout << "Foreign Agent: Sending registration reply to Mobile Node..." << endl << endl;
      Sleep(2);
   }
   
   // Otherwise, mobile IP is not needed
   else cout << "Mobile Node is in home network!" << endl;

}
