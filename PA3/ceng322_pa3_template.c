#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>

#define NUM_PLANES 9
#define NUM_AIRPORTS 3
#define BOARDING_TIME 3
#define TAKEOFF_TIME 2
#define LANDING_TIME 2
#define UNLOADING_TIME 2

time_t simulation_start;

int flying_times[3][3] = {
    {0, 3, 4},
    {2, 0, 2},
    {3, 5, 0}
};
// Airport 0 right corridor ->  Corridor 0
// Airport 1 right corridor ->  Corridor 1
// Airport 2 right corridor ->  Corridor 2
// Airport 0 left corridor ->  Corridor 2
// Airport 1 left corridor ->  Corridor 0
// Airport 2 left corridor ->  Corridor 1
// Columns represent airports, rows represent corridors. 0 for right corridor, 1 for left corridor.
int airport_corridor_connections[2][3] = {
    {0, 1, 2},  // right corridors
    {2, 0, 1}   // left corridors
};

// Satırlar: current_airport, Sütunlar: destination_airport
// -1 olan yerler aynı havalimanını temsil eder (uçuş yok).
int routing_map[3][3] = {
    {-1, 0, 2}, // Airport 0'dan 1'e -> Koridor 0, Airport 0'dan 2'ye -> Koridor 2
    {0, -1, 1}, // Airport 1'den 0'a -> Koridor 0, Airport 1'den 2'ye -> Koridor 1
    {2, 1, -1}  // Airport 2'den 0'a -> Koridor 2, Airport 2'den 1'e -> Koridor 1
};

//
sem_t gates_sem[NUM_AIRPORTS];
pthread_mutex_t takeoff_mutex[NUM_AIRPORTS];
pthread_mutex_t landing_mutex[NUM_AIRPORTS];    
pthread_mutex_t corridor_mutex[NUM_AIRPORTS];


struct Plane {
    int id;
    int current_airport;
    int destination_airport;
};



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

void handle_boarding(struct Plane* plane, sem_t* gate_sem){
    // Implementation for boarding
    print_status("Plane %d is waiting for boarding at Airport %d.\n", plane->id, plane->current_airport);
    sem_wait(&gate_sem[plane->current_airport]);
    print_status("Plane %d is boarding at Airport %d.\n", plane->id, plane->current_airport);
    sleep(BOARDING_TIME);
    sem_post(&gate_sem[plane->current_airport]);

}

int handle_takeoff(struct Plane* plane, pthread_mutex_t* takeoff_mutex){
    // Implementation for takeoff
    print_status("Plane %d is waiting for takeoff at Airport %d.\n", plane->id, plane->current_airport);
    pthread_mutex_lock(&takeoff_mutex[plane->current_airport]);
    int chosen_corridor = handle_airspace_clearance(plane, corridor_mutex);
    print_status("Plane %d is taking off from Airport %d.\n", plane->id, plane->current_airport);
    sleep(TAKEOFF_TIME);
    pthread_mutex_unlock(&takeoff_mutex[plane->current_airport]);
    return chosen_corridor;
}

handle_airspace_clearance(struct Plane* plane, pthread_mutex_t* corridor_mutex){
    // Implementation for airspace clearance
    print_status("Plane %d is waiting for airspace clearance at Airport %d.\n", plane->id, plane->current_airport);
    int right_corridor = airport_corridor_connections[0][plane->current_airport];
    int left_corridor = airport_corridor_connections[1][plane->current_airport];
    while(1){
        if(pthread_mutex_trylock(&corridor_mutex[right_corridor]) == 0){
            if(pthread_mutex_trylock(&corridor_mutex[left_corridor]) == 0){
                int chosen_corridor = routing_map[plane->current_airport][plane->destination_airport];
                if(chosen_corridor == right_corridor){
                    pthread_mutex_unlock(&corridor_mutex[left_corridor]);
                }else{
                    pthread_mutex_unlock(&corridor_mutex[right_corridor]);
                }
                return chosen_corridor;
            }else{
                pthread_mutex_unlock(&corridor_mutex[right_corridor]);
                usleep(1000);
            }
        }
    }
}

void handle_flying(struct Plane* plane){
    // Implementation for flying
    int duration = flying_times[plane->current_airport][plane->destination_airport];
    
    for(int i = 0; i < duration; i++){
        print_status("Plane %d is flying towards airport %d\n", plane->id, plane->destination_airport);
        sleep(1); // Toplam süreyi tek seferde değil, 1'er saniye uyutarak geçiriyoruz
    }
}

void handle_landing(struct Plane* plane, pthread_mutex_t* landing_mutex, pthread_mutex_t* corridor_mutex, int chosen_corridor){
    // Implementation for landing
    print_status("Plane %d is waiting for landing at Airport %d.\n", plane->id, plane->destination_airport);
    pthread_mutex_lock(&landing_mutex[plane->destination_airport]);
    print_status("Plane %d is landing at Airport %d.\n", plane->id, plane->destination_airport);
    sleep(LANDING_TIME);
    pthread_mutex_unlock(&corridor_mutex[chosen_corridor]);
    pthread_mutex_unlock(&landing_mutex[plane->destination_airport]);
}

void handle_unloading(struct Plane* plane){
    // Implementation for unloading
}

void* simulate_plane(void* arg){

    struct Plane* current_plane = (struct Plane*)arg;
    


    while(1){      
        do {
        current_plane->destination_airport = rand() % NUM_AIRPORTS;
        } while (current_plane->destination_airport == current_plane->current_airport);
        
        
        handle_boarding(current_plane, gates_sem);           
        handle_takeoff(current_plane, takeoff_mutex);           
        handle_flying(current_plane);             
        handle_landing(current_plane, landing_mutex, corridor_mutex, chosen_corridor);            
        handle_unloading(current_plane);
        
        current_plane->current_airport = current_plane->destination_airport;
    }                             
}

int main(){
    srand(time(NULL));
    simulation_start = time(NULL);
    
    // Initialize semaphores and mutexes
    for(int i = 0; i < NUM_AIRPORTS; i++){
        sem_init(&gates_sem[i], 0, 2); 
        pthread_mutex_init(&takeoff_mutex[i], NULL);
        pthread_mutex_init(&landing_mutex[i], NULL);
        pthread_mutex_init(&corridor_mutex[i], NULL);
    }

    // Create plane threads
    pthread_t planes[NUM_PLANES];
    struct Plane plane_data[NUM_PLANES];
    for (size_t i = 0; i < NUM_PLANES; i++)
    {
        plane_data[i].id = i;
        plane_data[i].current_airport = rand() % NUM_AIRPORTS;
        plane_data[i].destination_airport = rand() % NUM_AIRPORTS;
        while (plane_data[i].destination_airport == plane_data[i].current_airport) {
            plane_data[i].destination_airport = rand() % NUM_AIRPORTS;
        }
        pthread_create(&planes[i], NULL, simulate_plane, &plane_data[i]);
    }

    // Wait for plane threads to finish (in this simulation, they run indefinitely, so this is just for completeness)
    for (size_t i = 0; i < NUM_PLANES; i++)
    {
        pthread_join(planes[i], NULL);
    }
    

    return 0;
}