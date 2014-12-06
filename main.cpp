#include <iostream>
#include <string>
#include <list>
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

// Function Prototype Declarations
void registerMN();

// Main Simulation
int main()
{
   // TESTING
   homeAgent testHA("111.111.111.1");
   testHA.addEntry("111.111.111.1", "222.222.22.22", 3);
   testHA.addEntry("211.111.111.1", "322.222.22.22", 4);
   testHA.printEntries();
   cout << endl;
   foreignAgent testFA("222.222.222.2");
   testFA.addEntry("444.444.44.44", "555.555.55.5", "66-66-66-66-66-66",7);
   testFA.addEntry("544.444.44.44", "655.555.55.5", "76-66-66-66-66-66",8);
   testFA.printEntries();
   cout << endl;

   // Register Mobile Node to Host Agent
   registerMN();   


   return 0;
}

// Function Implementation
void registerMN()
{
   // Check if function is not at home network
   
      // Listen for broadcast
      
      // MN: send request to foreign agent

      // FA: relay request to host agent

      // FA: update visitor table
      
      // HA: send reply to foreign agent

      // HA: update binding table
      
      // MN: relay reply to foreign agent

   // Otherwise, mobile IP is not needed
   
}
