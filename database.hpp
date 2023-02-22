#ifndef DATABASE
#define DATABASE

#include <iostream>
#include <fstream>
#include <vector>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

// database class creates database object of specified schema

class DataBase{
    // private data members
    json data;  // for storing the data from file to manipulate
    std::string FileName = "users.json";  // specified file that is pre defined
    std::vector<std::string> schema;      // vector of schema for database
    int Size;   // contains size of the schema

    // binarySearch function takes sorted vector of json,
    // with key where the item is to be finded as value in form of {key: item}

    int binarySearch(std::vector<json> arr, std::string key, std::string item)
    {
        int left = 0;
        int right = arr.size()-1;
        while(left <= right)
        {
            int mid = (left+right)/2;
            if(arr[mid][key] == item)
            {
                return mid;
            }
            else if(arr[mid][key] < item)
            {
                left = mid+1;
            }
            else if(arr[mid][key] > item)
            {
                right = mid-1;
            }
        }
        return -1;
    }

    public:

        // constructor takes one parameter, that is vector of schema for database

        DataBase(std::vector<std::string> schema){
            this->schema = schema;
            this->Size = schema.size();
        }

        // time complexity is O(1)
        // opens the file and stores file data in "data" variable in json format

        void Open(){
            std::ifstream file(FileName);
            if (file.is_open()) {
                    file >> data;
                    file.close();
            }
        }

        // time complexity is O(1)
        // stores the manipulated data inside the database file and closes the file to save data

        void Save(){
            std::ofstream outfile(FileName);
            outfile << data.dump(2) << std::endl;
            outfile.close();
        }

        // time complexity is O(log n) due to red-black tree data structure in json library
        // creates collection of data w.r.t the values in schema
        // vector values contains values adjacent to the schema created in contructor
        // id is to be automatically generated by increamenting previous values

        json Create(std::vector<std::string> values) {
            if(values.size() != Size){
                std::cout << "data is not matching given schema\n";
                return NULL;
            }
            json user, temp;
            int id = 1;
            if(!data.empty()){
                sort(data.begin(), data.end(), [](json &a, json &b){
                    return a["id"] < b["id"];
                });
                temp = data.back();
                id = temp["id"];
                id++;
            }

            user["id"] = id;

            for(int i = 0 ; i < Size ; i++){
                user[schema[i]] = values[i];
            }

            data.push_back(user);
            std::cout << "Data created.\n"; 

            return user;
        }

        // time complexity is O(n*log n) for sort & binarySearch functions
        // displays the data on console in json formatof required user

        json Read(std::string key, std::string value){
            
            sort(data.begin(), data.end(), [key](json &a, json &b){
                return a[key] < b[key];
            });
            
            int userId = binarySearch(data, key, value);

            if(userId == -1){
                std::cout << "User not found.\n";
                return NULL;
            }

            json user = data[userId];

            std::cout << "User found.\n";
            return user;
        }

        // time complexity is O(n*log n) for sort & binarySearch functions
        // updates the data by finding the given {key: value} pairs as arguments

        json Update(std::string key, std::string value){
            
            sort(data.begin(), data.end(), [key](json &a, json &b){
                return a[key] < b[key];
            });

            int userId = binarySearch(data, key, value);
            
            if(userId == -1){
                std::cout << "user not found.\n";
                return NULL;
            }

            json user = data[userId];
            
            int choice;
            std::string element;

            std::cout << "Select option u want to update: \n";
            for(int i = 0 ; i < Size ; i++){
                std::cout << (i+1) <<". "<< schema[i] << std::endl;
            }

            std::cin >> choice;

            if(choice <= 0 || choice > Size){
                std::cout << "schema out of range.\n";
                return NULL;
            }
            std::cout << "Enter new value: ";
            std::cin >> element;

            user[schema[choice-1]] = element;

            data[userId] = user;

            std::cout << "Updated values: \n";

            std::cout << user.dump(2) << std::endl;

            return user;
        }

        // time complexity is O(n*log n) for sort and binarySearch function
        // deletes the data based on given {key: value} pair as arguments

        void Delete(std::string key, std::string value){
            
            sort(data.begin(), data.end(), [key](json &a, json &b){
                return a[key] < b[key];
            });

            int userId = binarySearch(data, key, value);

            if(userId == -1){
                std::cout << "User not found.\n";
                return;
            }

            data.erase(data.begin()+ userId);

            std::cout << "User have been deleted.\n";

            return;
        }
};

#endif