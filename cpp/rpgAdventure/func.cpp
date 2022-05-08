#include <iostream>
#include <string>
#include <iterator>
#include <bits/stdc++.h>
#include <vector>
#include <random>
#include <ctime> // for std::time

#include "vars.h"

void pickUpWeapon(std::string weapon);

//get string input from player
std::string getString(std::string prompt)
{
    std::cout << prompt;
    std::string x{};
    std::getline(std::cin >> std::ws, x);

    return x;
}

std::string getGender()
{
    int input{};
    std::string gender{};
    while(true)
    {
        std::cout << "Gender of character: \n1 male\n2 female\n3 other\ninput: ";
        std::cin >> input;

        switch (input)
        {
        case 1:
            break;
        case 2:
            break;
        case 3:
            std::cout << "please specify: ";
            std::getline(std::cin >> std::ws, gender);
            return gender;
        default:
            continue;
        }
        break;
    }

    switch (input)
    {
    case 1:
        gender = "Male";
        break;
    case 2:
        gender="female";
        break;
    default:
        break;
    }
    
    return gender;
}

void getStats(int points)
{
    while(points>0)
    {
        std::cout << "You have " << points << " points:\n[1]strength " << p::perks["strength"] << "\n[2]Defence " << p::perks["defence"] << "\n[3]Vitality " << p::perks["vitality"] << "\n[4]Intelligence " << p::perks["intelligence"] << "\n[5]Agility " << p::perks["agility"] << "\nspend point on what: ";

        int input{};
        std::cin >> input;
        if (input == 1 || input == 2 || input == 3 || input == 4 || input == 5)
        {
            std::cout << "You have chosen " << p::perkNames[input-1] << '\n' << "How many points do you want to spend?(" << points << " points left): ";
            int x;
            while (true)
            {
                std::cin >> x;
                if (x<=points)
                {
                    p::perks[p::perkNames[input-1]]+=x;
                    points -= x;
                    break;
                } else 
                {
                    std::cout << "You dont have enough points!" << '\n' << "How many points do you want to spend?: ";
                }
            }
        }
    }
}

void getStarterWeapon()
{
    int input{};
    while (true)
    {
        std::cout << "Choose starting weapon:" << "\n[1]Sword" << "\n[2]axe" << "\n[3]bow" << "\nInput: ";
        std::cin >> input;
        switch (input)
        {
        case 1:
            pickUpWeapon("sword");
            break;
        case 2:
            pickUpWeapon("axe");
            break;
        case 3:
            pickUpWeapon("bow");
            break;
        case 4:
            pickUpWeapon("blessedSword");
            break;
        
        default:
            continue;
        }
        break;
    }
    
}

void getType()
{
    std::cout << "Freely choose your race(orc, human, etc.): ";
    std::string x;
    getline(std::cin >> std::ws, x);

    c::race = x;
}

void printBasicInfo()
{
    std::cout << '\n' << "Name: " << c::name << "\nGender: " << c::gender << "\nRace: " << c::race <<"\nEquipped weapon: " << w::weaponNames[i::weapons[0]];
}

//prints perks and stats
void printPS()
{
    std::cout << "Perks:\t\t\t\tStats:" << "\nstrength: " << p::perks["strength"] << "\t\t\tDamage: " << p::damage << "\nDefence: " << p::perks["defence"] << "\t\t\tSpeed: " << p::speed << "\nVitality: " << p::perks["vitality"] << "\t\t\thp: " << p::currentHp << "/" << p::hp << "\nIntelligence: " << p::perks["intelligence"] << "\t\t\tDamage resistance: " << p::damageResistance << "\nAgility: " << p::perks["agility"] << '\n';
}

//prints currently equipped armor
void printArmor()
{
    std::cout << "Armor\t\t\tDefence\n" << a::helmetNames[i::helmet] << "\t\t" << a::helmets[a::helmetNames[i::helmet] + "Defence"] << '\n' << a::chestplateNames[i::chestplate] << '\t' << a::chestplates[a::chestplateNames[i::chestplate] + "Defence"] << '\n' << a::leggingNames[i::leggings] << "\t\t" << a::leggins[a::leggingNames[i::leggings] + "Defence"] << '\n' << a::bootNames[i::boots] << "\t\t" << a::boots[a::bootNames[i::boots] + "Defence"] << "\n\n";
}

void printCharacterCard()
{
    if(i::weapons[0]==1)
    {
        std::cout << art::playerSword;
    } else if(i::weapons[0]==2)
    {
        std::cout << art::playerAxe;
    } else if (i::weapons[0]==3)
    {
        std::cout << art::playerBow;
    } else
    {
        std::cout << "art not added yet\n";
    }

    printBasicInfo();
    std::cout << "\n\n";
    printPS();
    std::cout << '\n';
    printArmor();
}

//initalise players stats
void initStats()
{
    p::damage = p::perks["strength"] + w::weapons[w::weaponNames[i::weapons[0]] + "Damage"];

    int weaponWeightEffect{w::weapons[w::weaponNames[i::weapons[0]] + "Weight"] - p::perks["strength"]};
    if (w::weapons[w::weaponNames[i::weapons[0]] + "Weight"] - p::perks["strength"]<=0)
    {
        weaponWeightEffect = 0;
    }

    p::speed = p::baseSpeed + p::perks["agility"] - weaponWeightEffect;

    int armorDefence{a::helmets[a::helmetNames[i::helmet] + "Defence"] + a::chestplates[a::chestplateNames[i::chestplate] + "Defence"] + a::leggins[a::leggingNames[i::leggings] + "Defence"] + a::boots[a::bootNames[i::boots] + "Defence"]};
    p::damageResistance = p::baseDamageRes + p::perks["defence"] + armorDefence;
    p::hp = p::baseHp + p::perks["vitality"];

    p::currentHp = p::hp;
}

void createCharacter()
{
    //get name of players charecter
    c::name = getString("Name of character: ");
    //get gender of players character
    c::gender = getGender();
    //get stats
    getStats(15);
    //get starter weapon
    getStarterWeapon();
    //get character type
    getType();

    //initiate the stats
    initStats();

    printCharacterCard();
}

bool combat(std::string enemy)
{
    std::cout << "\nYou are fighting " << e::enemyNames[e::enemies[enemy + "Id"]] << '\n';
    std::cout << "Your hp: " << p::currentHp << "/" << p::hp << "\n" << e::enemyNames[e::enemies[enemy + "Id"]] << "s hp: " << e::enemies[enemy + "CurrentHp"] << "/" << e::enemies[enemy + "Hp"] << "\n\n";

    //checks who plays first based on speed
    bool playerFirst{};

    if(p::speed > e::enemies[enemy + "Speed"])
    {
        playerFirst = true;
    } else
    {
        playerFirst = false;
    }

    //finds out how much damage each does to eachother and makes sure its not negative
    int playerTempDamage{};
    if (p::damage - e::enemies[enemy + "DamageResistance"]<=0)
    {
        playerTempDamage = 1;

    } else if(p::damage - e::enemies[enemy + "DamageResistance"]<=-10)
    {
        playerTempDamage = 0;

    } else
    {
        playerTempDamage = p::damage - e::enemies[enemy + "DamageResistance"];
    }

    int enemyTempDamage{};
    if (e::enemies[enemy + "Damage"] - p::damageResistance<=0)
    {
        enemyTempDamage = 1;

    } else if(e::enemies[enemy + "Damage"] - p::damageResistance<=-10)
    {
        enemyTempDamage = 0;

    } else
    {
        enemyTempDamage = e::enemies[enemy + "Damage"] - p::damageResistance;
    }

    // Initialize our mersenne twister with a random seed based on the clock
    std::mt19937 mersenne{ static_cast<std::mt19937::result_type>(std::time(nullptr)) };

    // Create a reusable random number generator that generates uniform numbers between 1 and 6
	std::uniform_int_distribution die{ 1, 6 };

    //comabt loop
    while (true)
    {
        

        if (playerFirst && p::currentHp > 0)
        {
            std::cout << "player attacks first!";

            std::cout << "roll 4-6 to hit. Roll die: ";
            getc(stdin);
            int dieResult{die(mersenne)};
            std::cout << dieResult << '\n';

            if(4<=dieResult && dieResult<=6)
            {
                std::cout << "Hit!\n";
                e::enemies[enemy + "CurrentHp"] -= playerTempDamage;
                std::cout << "You did " << playerTempDamage << " damage\n";
            } else 
            {
                std::cout << "Miss!\n";
            }

            //checks if enemy is already dead
            if (e::enemies[enemy + "CurrentHp"] > 0)
            {
                std::cout << "Roll 4-6 to dodge. Press enter to roll dies: ";
                getc(stdin);
                int dieResultDodge{die(mersenne)};
                std::cout << dieResultDodge << '\n';

                if(1<=dieResultDodge && dieResultDodge<=3)
                {
                    std::cout << "Hit!\n";

                    p::currentHp -= enemyTempDamage;
                    std::cout << e::enemyNames[e::enemies[enemy + "Id"]] << " did " << enemyTempDamage << " damage\n";
                } else 
                {
                    std::cout << "Dodged!\n";
                }
            } else
            {
                break;
            }
        }else if(e::enemies[enemy + "CurrentHp"] > 0)
        {
            std::cout << enemy << " attacks first!";

            std::cout << "Roll 4-6 to dodge. Press enter to roll dies: ";
            getc(stdin);
            int dieResultDodge{die(mersenne)};
            std::cout << dieResultDodge << '\n';

            if(1<=dieResultDodge && dieResultDodge<=3)
            {
                std::cout << "Hit!\n";

                p::currentHp -= enemyTempDamage;
                std::cout << e::enemyNames[e::enemies[enemy + "Id"]] << " did " << enemyTempDamage << " damage\n";
            } else 
            {
                std::cout << "Dodged!\n";
            }

            //checks if payer is already dead
            if(p::currentHp > 0)
            {
                std::cout << "roll 4-6 to hit. Roll die: ";
                getc(stdin);
                int dieResult{die(mersenne)};
                std::cout << dieResult << '\n';

                if(4<=dieResult && dieResult<=6)
                {
                    std::cout << "Hit!\n";

                    e::enemies[enemy + "CurrentHp"] -= playerTempDamage;
                    std::cout << "You did " << playerTempDamage << " damage\n";
                } else 
                {
                    std::cout << "Miss!";
                }
            } else
            {
                break;
            }
        } else
        {
            break;
        }
        

        //displays eachs hp
        std::cout << "Your hp: " << p::currentHp << "/" << p::hp << "\n" << e::enemyNames[e::enemies[enemy + "Id"]] << "s hp: " << e::enemies[enemy + "CurrentHp"] << "/" << e::enemies[enemy + "Hp"] << '\n';
    }

    //displays who won
    if (p::currentHp <= 0)
    {
        std::cout << "You lost." << "\nEnemys remaining hp: " << e::enemies[enemy + "CurrentHp"] << "/" << e::enemies[enemy + "Hp"] << '\n';
        return false;
    } else
    {
        std::cout << "You won!" << "\nYour hp: " << p::currentHp << "/" << p::hp << '\n';
        return true;
    }

    e::enemies[enemy+"currentHp"] = e::enemies[enemy + "Hp"];
}

void pickUpWeapon(std::string weapon)
{
    //checks the length of weapon inventory array
    int inventorySpace{sizeof(i::weapons) / sizeof(i::weapons[0])};
    //checks for empty inventory spaces and puts the weapon in if it finds one
    for(int x{0}; x < inventorySpace; ++x)
    {
        if(i::weapons[x]==0)
        {
            i::weapons[x]=w::weapons[weapon + "Id"];

            std::cout << weapon << " was put in slot " << x+1 << " of weapon inventory\n";
            initStats();
            return;
        }
    }

    std::cout << "No space in inventory!\nWeapons in inventory: ";
        for (int x{0}; x< inventorySpace; ++x)
        {
            std::cout << w::weaponNames[i::weapons[x]] << ", ";
        }
    //checks if player wants to switch weapon out if they have no ampy space
    std::cout << "Do you want to switch it out with another weapon?y/n: ";
    bool switchWeapon{};
    while (true)
    {
        char input{};
        std::cin >> input;

        switch (input)
        {
        case 'y':
            switchWeapon = true;
            break;
        case 'n':
            switchWeapon = false;
            break;
    
        default:
            continue;
        }
        break;
    }

    //switches a weapon out if they said yes
    if(switchWeapon)
    {
        while(true)
        {
            std::cout << "\nWich weapon do you want to discard?1-" << inventorySpace << ": ";
            int input{};
            std::cin >> input;

            if (input == 1 || input == 2 || input == 3 || input == 4 || input == 5)
            {
                i::weapons[input-1]=w::weapons[weapon + "Id"];
                break;
            } else
            {
                std::cout << "Invalid input!" << '\n';
            }
        }

        std::cout << "Weapons in inventory: ";
        for (int x{0}; x< inventorySpace; ++x)
        {
            std::cout << w::weaponNames[i::weapons[x]] << ", ";
        }
        initStats();
    } else
    {
        std::cout << "Throwing away " << weapon << '\n';
    }
}

void printItemInventory(int inventorySpace)
{
    std::string preItem{};
    int repCounter{1};

    bool first{false};

    for (int x{0}; x<inventorySpace; ++x)
    {
        if(first)
        {
            if(preItem!=it::itemNames[i::items[x]])
            {
                std::cout << preItem << ", ";
            }
            first = false;
        }

        if(it::itemNames[i::items[x]] == preItem)
        {
            ++repCounter;
            if(x==34)
            {
                std::cout << repCounter << "x " << preItem << '\n';
            }
        }else if(repCounter>1)
        {
            std::cout << repCounter << "x " << preItem << ", " <<it::itemNames[i::items[x]] << ", ";
        }else
        {
            first = true;
        }

        preItem = it::itemNames[i::items[x]];
    }
//    for (int x{0}; x< inventorySpace; ++x)
//    {
//        if((x+1)%5==0)
//        {
//            std::cout << it::itemNames[i::items[x]] << '\n';
//        } else 
//        {
//            std::cout << it::itemNames[i::items[x]] << ", ";
//        }
//    }
    std::cout << '\n';
}

void pickUpItem(std::string item)
{
    //checks the length of item inventory array
    int inventorySpace{sizeof(i::items) / sizeof(i::items[0])};

    std::cout << "Items in inventory: " << '\n';
    printItemInventory(inventorySpace);
//    for (int x{0}; x< inventorySpace; ++x)
//    {
//        if((x+1)%5==0)
//        {
//            std::cout << it::itemNames[i::items[x]] << '\n';
//        } else 
//        {
//            std::cout << it::itemNames[i::items[x]] << ", ";
//        }
//    }
    //checks for empty inventory spaces and puts the weapon in if it finds one
    for(int x{0}; x < inventorySpace; ++x)
    {
        if(i::items[x]==0)
        {
            i::items[x]=it::items[item + "Id"];

            std::cout << item << " was put in slot " << x+1 << " of item inventory\n";
            initStats();
            printItemInventory(inventorySpace);

            return;
        }
    }
    std::cout << "No space in inventory!\n";
    //checks if player wants to switch item out if they have no ampy space
    std::cout << "Do you want to switch it out with another item?y/n: ";
    bool switchItem{};
    while (true)
    {
        char input{};
        std::cin >> input;

        switch (input)
        {
        case 'y':
            switchItem = true;
            break;
        case 'n':
            switchItem = false;
            break;
    
        default:
            continue;
        }
        break;
    }

    //switches a item out if they said yes
    if(switchItem)
    {
        while(true)
        {
            std::cout << "\nWich item do you want to discard?1-" << inventorySpace << ": ";
            int input{};
            std::cin >> input;

            if (1 <= input && input <= inventorySpace)
            {
                i::items[input-1]=it::items[item + "Id"];
                break;
            } else
            {
                std::cout << "Invalid input!" << '\n';
            }
        }
        
        std::cout << "Items in inventory: " << '\n';
        printItemInventory(inventorySpace);

        initStats();
    } else
    {
        std::cout << "Throwing away " << item << '\n';
    }
}

void equipWeapon()
{
    //checks the length of weapon inventory array
    int inventorySpace{sizeof(i::weapons) / sizeof(i::weapons[0])};

    std::cout << "Weapons on inventory: " << '\n';
    for (int x{0}; x< inventorySpace; ++x)
    {
        std::cout << w::weaponNames[i::weapons[x]] << ", ";
    }

    while(true)
    {
        std::cout << "Wich weapon do you want to equip?[1-" << inventorySpace << "]?: " << '\n';
        int input{};
        std::cin >> input;

        if (input == 1 || input == 2 || input == 3 || input == 4 || input == 5)
        {
            int temp{i::weapons[0]};
            i::weapons[0] = input;
            i::weapons[1] = temp;
            break;
        } else
        {
            std::cout << "Invalid input!" << '\n';
        }
    }

    std::cout << "Weapons on inventory: " << '\n';
    for (int x{0}; x< inventorySpace; ++x)
    {
        std::cout << w::weaponNames[i::weapons[x]] << ", ";
    }

    initStats();
    std::cout << '\n';
}

void usePotion(std::string potion)
{
    int potionInt{it::items[potion + "Id"]};
    switch (potionInt)
    {
        case 1:
            p::currentHp += it::items[potion + "Effect"];
            if (p::currentHp > p::hp)
            {
                p::currentHp = p::hp;
            }
            break;
        default:
            std::cout << "Not added yet or invalid!" << '\n';
            break;
    }
}

void useItem()
{
    //checks the length of item inventory array
    int inventorySpace{sizeof(i::items) / sizeof(i::items[0])};
    std::cout << "Items in inventory: " << '\n';
    for (int x{0}; x< inventorySpace; ++x)
    {
        if((x+1)%5==0)
        {
            std::cout << it::itemNames[i::items[x]] << '\n';
        } else 
        {
            std::cout << it::itemNames[i::items[x]] << ", ";
        }
    }

    

    while(true)
    {
        std::cout << "\nWich item do you want use?[1-" << inventorySpace << "]?: " << '\n';
        int input{};
        std::cin >> input;

        if (1 <= input && input <= inventorySpace)
        {
            usePotion("healingPotion");
            break;
        } else
        {
            std::cout << "Invalid input!" << '\n';
        }
    }
}

void search(std::string items, std::string adjectives, std::string noun)
{
    std::cout << "You look around the " << adjectives << noun <<" , and find: ";
    
    std::vector<std::string> itemList;
    std::string del{","};

    int start = 0;
    int end = items.find(del);
    while (end != -1) {
        std::string temp{items.substr(start, end - start)};
        start = end + del.size();
        end = items.find(del, start);
        itemList.push_back(temp);
    }

    for (int x {0}; x < itemList.size(); ++x)
    {
        if (x==itemList.size()-1)
        {
            std::cout << itemList[x] << '\n';
            break;
        }
        
        std::cout << itemList[x] << ", ";
    }

    for (int x {0}; x < itemList.size(); ++x)
    {
        pickUpItem(itemList[x]);
    }
}

void firstHallway(bool first);

void gameBeginning(bool first)
{
    if(first)
    {
        std::cout << "You wake up, in a dark, damp room, laying on the ground. You can see a dim light down the hallway. ";
    }
    
    std::string input{};

    while(true)
    {
        std::cout << "What do you want to do?: ";
        std::getline(std::cin >> std::ws, input);
        if (input=="go down the hallway")
        {
            std::cout << "You walked down the hallway.\n";
            if(!lS::beatFirstEncounter)
            {
                firstHallway(true);
            } else
            {
                firstHallway(false);
            }
            first = false;
        } else if (input=="search room" && !lS::room1Searched)
        {
            search("bloodRing,", "dark, damp", "room");
            lS::room1Searched = true;
        } else
        {
            if (input=="search room" && lS::room1Searched)
            {
                std::cout << "Room already searched\n";
            } else
            {
                std::cout << "You cannot " << input << '\n';
            }
        }
    }
}

void firstOrcEncounter(bool satDown);

void firstHallway(bool first)
{
    if(first)
    {
        std::cout << "You walk in to a warm room. It is brightly lit with torches lining the walls.\nThere is a fire in the midle with rocks, seemingly used as chairs, around it.\nIn the far right corner of the room is a wooden desk. To the left of the desk is a door.\n";
    } else
    {
        std::cout << "You killed the orc\n";
    }

    std::string input{};

    while (true)
    {
        std::cout << "What do you want to do?: ";
        std::getline(std::cin >> std::ws, input);

        if (input=="open door")
        {
            std::cout << "You opened the door.\n";
            firstOrcEncounter(false);
            break;
        } else if (input=="search desk" && !lS::searchedDesk1)
        {
            search("secondRoomKey,", "wooden", "desk");
            std::cout << "On the desk is a piece of parchment. It is filled with text in a language you cannot understand. It looks like a letter.\n";
            lS::searchedDesk1 = true;
        } else if (input=="sit down")
        {
            std::cout << "You sat down.\n";
            firstOrcEncounter(true);
            break;
        }else if(!first && "go back")
        {
            break;
        } else
        {
            if (input=="search desk" && lS::searchedDesk1)
            {
                std::cout << "Desk already searched\n";
            } else
            {
                std::cout << "You cannot " << input << '\n';
            }
        }
    }
}

void firstOrcEncounter(bool satDown)
{
    bool ifWon{};
    //if player opened the door
    if(!satDown)
    {
        std::cout << "On the other side of the door is another hallway with a greensskinned human-looking being walking down it. Suddenly it spot you and yells something icomprehensible. It starts sprinting down towards you. You slam the door the door and starts running back, but before you can get out of the room it is already trough the door. It raises its axe and you raise your " << w::weaponNames[i::weapons[0]] << '\n';

        ifWon = combat("orc");
    }
    //if player sat down 
    else if(satDown)
    {
        std::cout << "As you rest your legs on one of the rocks a greensskinned human-looking being walks in trough the door. It raises its axe and you raise your " << w::weaponNames[i::weapons[0]] << '\n';
        ifWon = combat("orc");
    }

    if(ifWon)
    {
        lS::beatFirstEncounter = true;
        firstHallway(false);

    } else if(!ifWon)
    {
        std::cout << "you are dead\n";
    }
}

