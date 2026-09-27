#include<iostream>
using namespace std;

int main(){
	int a[1000];
	int i = 0;
	int n = 0;

		while (i < 10000) {
			if (i % 13 == 0) {
				a[n] = i;
				n++;
			}
			i++;
				

}
	
}




















#include<vector>
#include<iostream>

using namespace std;
int main() 
{
	vector<int>v;
	int i=0;

	while(i<10000){



		if (i % 13 == 0) {
			v.push_back(i);

		}
	
	
		i++;
	}

	for (const auto& i : v)cout << i << " ";
	return 0;

}



#include<iostream>
#include<vector>
using namespace std;
int main() {
	vector<int>v;
	int i = 1;
	while (i <= 100) {

		
			v.push_back(i);
		

		
		i++;
		
	}


	for(auto it=v.begin();it!=v.end();)

		if (*it % 2 != 0) {
			it=v.erase(it);
		}
		else {
			++it;
		}

	for (const auto& i : v)
		cout << i<<" ";



	

}