// utils.cpp
#include "utils.h"
using namespace std;
namespace func{
string addSurname(const string& name) {
    return name + " Bobro";
}



string numbersMinMax(const string& numbers) {
    istringstream iss(numbers);
    vector<double> nums;
    int num;

    while (iss >> num){
        nums.push_back(num);
    }

    if (nums.empty()){
        return "No numbers";
    }

    nums.erase(nums.begin());

    if (nums.empty()){
        return "No valid numbers after 1";
    }

    int minNum = nums[0], maxNum = nums[0];

    for (int n : nums){
        if (n < minNum) minNum = n;
        if (n > maxNum) maxNum = n;
    }

    return to_string(maxNum) + " " + to_string(minNum);
}



string add_arithmetic_mean(const string& numbers) {
    istringstream iss(numbers);
    vector<double> nums;
    double num;

    while (iss >> num) {
        nums.push_back(num);
    }

    if (nums.empty()) {
        return "No numbers";
    }
    nums.erase(nums.begin());

    double mean = ((nums[0] + nums[1] + nums[2] + nums[3] ) / 4);
    
    for (double& n : nums) {
        n += mean;
    }

    ostringstream oss;
    for (int n : nums) {
        oss << n << " ";
    }

    return oss.str();
}



string checkPunctuation(const string& message) {
    for (size_t i = 1; i < message.size(); ++i) { // Починаємо з індексу 1
        if (ispunct(static_cast<unsigned char>(message[i]))) {
            return "Yes";
        }
    }
    return "No";
}
}