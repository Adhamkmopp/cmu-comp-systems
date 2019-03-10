#include <stdio.h>
#include <string.h>
#include <stdexcept>

void good_echo(){
    char *buff;
    int max = 10;
    fgets (buff, max, stdin);
    if ( buff == NULL) {
        throw std::invalid_argument( "received non-character values" );
    }

    if ( *buff == '\n') {
        throw "no input";
    }

    fputs(buff, stdout);

}

int main (){
    printf("Type something up to 10 characters long");
     try{
        good_echo();
    } catch(class std::invalid_argument &err){
         printf("%s", err.what());
    } catch(char const* err){
        printf("%s", err);
    }

}