#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_PLANES 9
#define NUM_AIRPORTS 3
#define BOARDING_TIME 3
#define TAKEOFF_TIME 2
#define LANDING_TIME 2
#define UNLOADING_TIME 2

time_t simulation_start;

/*/
/   Helper function for simulation time. You do not have to modify it.
*/
int get_simulation_time() {
    return (int)(time(NULL) - simulation_start);
}


/*
/   Helper function for printing the logs. You may have to modify it.
*/
void print_status(const char* message, int plane_id, int airport_id){
    printf("[TIME %d] ", get_simulation_time());
    printf(message, plane_id, airport_id);
}


/*
/   You should carefully decide what the return type and argument(s) of each following function
/   should be but be aware that there is no single correct answer. If what you think is 
/   working and makes sense, that's acceptable. You may, and encouraged to, introduce 
/   additional helper functions to increase modularity and readability of your code.
/   However not using any of the 'handle_*' functions is not suggested.
*/

handle_boarding();
handle_takeoff();
handle_flying();
handle_landing();
handle_unloading();

void* simulate_plane(void* arg){

    while(1){                        
        handle_boarding           
        handle_takeoff            
        handle_flying             
        handle_landing            
        handle_unloading          
    }                             
}

int main(){
    srand(time(NULL));
    simulation_start = time(NULL);

    return 0;
}