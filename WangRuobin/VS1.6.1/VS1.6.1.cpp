#include <iostream>
int main() {

    int arr[7];
    for (int i=1;i<=100;i++) {
        if (i%13==0){
            arr[i/13-1] = i;
        }
    }
    for (int j=0;j<7;j++) {
        std::cout << arr[j] << std::endl;
    }
}