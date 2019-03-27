void nothing();

int x_global = 123;
int z_global = 433;
extern int y_external;

static int z_static;
int main(){
    nothing();
    y_external+=1;
    x_global=1;
    return 1;

}