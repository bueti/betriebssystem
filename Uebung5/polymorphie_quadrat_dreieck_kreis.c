#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/time.h>


#define PI 3.14159265358979323846

typedef struct shape shape;
struct shape {
    int type; //1 = square, 2 = circle, 3 = triangle
    double (*area)(shape *);
    double (*circumference)(shape *);
    int a;
    int b;
    int c;
    int r;
};


void usage(char *argv0, char *text) {
    printf("%s\n", text);
    printf("\n\nUsage:\n\n%s:type param1 param2 param3\n\ntype:\n0 = demo calculation for multiple shapes (count of shapes from each type, length, radius and side length are more ore less random ;) ))\n1 = square (param1 = side length)\n2 = circle (param1 = radius)\n3 = triangle (param1 = side length 1, param2 = side length 2, param3 = side length 3)\n\n", argv0);
    exit(1);
}

double area( shape *s ){
    return s->area(s);
}

double area_square( shape *s ){
    return s->a * s->a;
}

double area_circle( shape *s ){
    return pow( s->r,2.0);
}

double area_triangle ( shape *s ){
    double t = (s->a+s->b+s->c)/2;
    return sqrt(t * (t - s->a) * (t-s->b) * (t-s->c) );
}

double circumference_square( shape *s ){
    return 4* s->a;
}

double circumference_circle( shape *s ){
    return PI * 2 * s->r;
}

double circumference_triangle( shape *s ){
    return s->a + s->b + s->c;
}

void create_square( shape *s,int  a ){
    s->area = &area_square;
    s->circumference = &circumference_square;
    s->a = a;
    printf("created square with side length %d\n",a);
}

void create_circle( shape *s, int r ){
    s->area = &area_circle;
    s->circumference = &circumference_circle;
    s->r = r;
    printf("created circle with radius length %d\n",r);
}

void create_triangle( shape *s, int a , int b , int c ){
    s->area = &area_triangle;
    s->circumference = &circumference_triangle;
    s->a = a;
    s->b = b;
    s->c = c;
    printf("created triangle with side lengths %d,%d,%d\n",a,b,c);
}



int main(int argc, char *argv[]) {

   // printf("welcome %d\n", argc);
    if( argc == 1 ){
         usage(argv[0], "wrong number of parameters");
    }
    int  p1  = strtol(argv[1], NULL, 10);

    //special:
    if( p1 == 0 ){

        int l = strtol(argv[2], NULL, 10);

        int i;
        struct shape *shapes[l*3-1];

        //create array:
        for(i = 0; i < l; i++){
            struct shape *s = (struct shape *)malloc(sizeof(struct shape));
            shapes[i] = s;

            create_square(s,i);
        }
        //create array:
        for(i = l; i < l*2; i++){

             struct shape *s = (struct shape *)malloc(sizeof(struct shape));
            shapes[i] = s;
            create_circle(s,i);

        }
        //create array:
        for( i = l*2; i< l*3 ; i++ ){
            struct shape *s = (struct shape *)malloc(sizeof(struct shape));
            shapes[i] = s;
            create_triangle(s,i,i,i);
        }

        //calculate circumference and area of all shapes:
        double circumference = 0;
        double area = 0;
        for(i = 0; i < 3*l; i++){
            area += shapes[i]->area(shapes[i]);
            circumference += shapes[i]->circumference(shapes[i]);
            free( shapes[i] );
        }
        printf("total area: %f\n",area);
        printf("total circumference: %f\n", circumference);
        exit(0);
    }

    //square:
    if (p1 == 1  && argc != 3) {
    usage(argv[0], "wrong number of parameters");
    }
    //circle
    if (p1 == 2 && argc != 3) {
       usage(argv[0], "wrong number of parameters");
    }

    //triangle
    if (  p1 == 3 && argc != 5) {
        usage(argv[0], "wrong number of parameters");
    }

    struct shape *s = (struct shape *)malloc(sizeof(struct shape));

    switch (p1) {
        case 1:
            create_square(s,strtol(argv[2], NULL, 10));
            break;
        case 2:
            create_circle(s,strtol(argv[2], NULL, 10));
            break;
        case 3:
            create_triangle(s,strtol(argv[2], NULL, 10),strtol(argv[3], NULL, 10),strtol(argv[4], NULL, 10));
            break;
        default:
            break;
    }

    printf("area is %f\n\n",s->area(s));
    printf("circumference is %f\n\n",s->circumference(s));
    free( s );
    exit(0);
}
