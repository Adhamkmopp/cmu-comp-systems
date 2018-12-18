
### 2.5.7

using namespace std;

void test_int(string &start, size_t len){
    int i;
    int e=0;
    for (i=0; i < len; i++)
    {
    if(start[i] == 0x2e || start[i] <0x30 || start[i] > 0x39 ) throw e;
    
    }
   
   
}


int main()

{
    string anyNumber;
    cout << "Input any number\n";
    cin >> anyNumber;
    try{
        test_int(anyNumber, anyNumber.length());


    } catch(int notInt)
    {
        cout << "Not an integer, exiting\n";
        exit(0);
    }

    test_show_bytes(stoi(anyNumber));
  

}

### 2.5.8
int is_little_indian(){
    int test_byte = 0xff;
    byte_pointer start = (byte_pointer) &test_byte;
    if (start[0] == 0xff){
        return 0;

    } else {
        return 1;
    }

}
int main (){
int whatis = is_little_indian();
cout << (( whatis == 0) ? "This machine is Little Endian" : "This machine is Big Endian");


}

### 2.6.0

int replace_byte(unsigned &old_hex, int loc, unsigned rep_hex){
    byte_pointer new_hex = (byte_pointer) &old_hex;
    new_hex[loc] = rep_hex;
}
int main (){
    unsigned x = 0x12345678;
cout << "Previously, the old hex was "; 
show_bytes((byte_pointer) &x, sizeof(unsigned)); 
cout << "The new hex is "; replace_byte(x, 2, 0xAB);
show_bytes((byte_pointer) &x, sizeof(unsigned)); 

}

### 2.6.1

int main (){
int anyNum;
cout << "Input any number including zero" << endl;
cin >> anyNum;
int bit_test = anyNum ^ 0x0;
bool bits = !!bit_test;
cout << "Number includes a 1 somewhere:" << bits << endl;

bit_test = anyNum ^ ~0x0;
bits = !!bit_test;
cout << "Number includes a 0 somewhere:" <<  bits << endl;

int shift_val = (sizeof(int)-1)<<3;
char msb = (char) (anyNum >> shift_val);
char lsb = (char) anyNum;
char bit_test_new = lsb ^ 0x00;
bits = !!bit_test_new;

cout << "Number includes a 1 in the least signfificant bit:" << bits<< endl;

bit_test_new = msb ^ 0x00;
bits = !!bit_test_new;
cout<< msb;
cout << "Number includes a 1 in the most signfificant bit:" << bits<< endl;

}

### 2.6.2
int int_shifts_are_arithmetic (){
    int testNum= -1;
    int shift_size = ((sizeof(int) -1) << 3) + 7;

    int newNum = testNum >> shift_size;
    cout << newNum << endl;
    bool res;
    res = (newNum == testNum);
    return res;

}


int main (){

int endresults = int_shifts_are_arithmetic();
    cout << "This machine performs uses right shifts that are: "<<endl;
    cout << "Arithmetic: " << endresults << endl;
    cout << "Logical: " << !endresults <<endl;


}

### 2.6.3

unsigned srl (unsigned x, int k){
    unsigned xsra = (int) x >> k;
    int size = sizeof(int) * 8;
    return (xsra & ~(0x1 << (size -1)) >> k);
}

signed sra(int x, int k){
    int xsrl = (unsigned) x>>k;
    int size = sizeof(int) * 8;
    return (xsrl | ~(0x1 << (size -1)) >> k);
}

### 2.6.4

int any_odd_one(unsigned x){
return ( (x | 0xAAAAAAAA) == 0xAAAAAAAA);
}

int main (){

int anyNum;
cout << "Input any number including zero" << endl;
cin >> anyNum;
int bit_test = any_odd_one(anyNum);
cout << "Number includes an odd 1 bit somewhere:" << bit_test << endl;
}
