
long switch_prob (long x, long n){
    long result = x;
    switch(n){

        case 60:
        case 63:
            result = 8 * x;
            break;
        case 64:
            result = x >>3;
            break;
        case 65:
            result = (x << 4) - x;
            result *= result;
        default:
            result = x + 75; 

    }
    return result;
}