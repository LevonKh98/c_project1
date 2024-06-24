#include <stdio.h>
#include <stdlib.h>

// Declare dynamic arrays/vectors and global variables
int *resource = NULL;
int *available = NULL;
int *maxclaim = NULL;
int *allocated = NULL;
int *need = NULL;

int num_processes;
int num_resources;

//*********************************************************
void printVector(int *vector, char *title) {
	// declare local variables 
	
	printf("\n%s:\n", title);
	
	// for loop j: print each resource index 
	for(int j = 0; j < num_resources; j++)
	{
	    printf("\tr%d", j);
	}
	printf("\n");
	
	// for loop j: print value of vector[j] 
	for(int j = 0; j < num_resources; j++)
	{
	    printf("\t%d", vector[j]);
	}
	printf("\n");
	
	return;
}

//*************************************************************
void printMatrix(int *matrix, char *title) { 
	// declare local variables 
	
	printf("\n%s:\n", title);
	
	// for loop j: print each resource index 
	for(int j = 0; j < num_resources; j++)
	{
	    printf("\tr%d", j);
	}
	printf("\n");
	
	// for each process i: 
	for (int i = 0; i < num_processes; i++)
	{
	    printf("p%d", i);
	    // for each resource j: 
	    for(int j = 0; j < num_resources; j++)
	    {
	        // print value of matrix[i,j]
	        printf("\t%d", matrix[i * num_resources + j]);
	    }
		printf("\n");
	}
	
	return;
}


//**************************************************************
void option1() {
    // declare local variables 
    
	// prompt for number of resources 
	printf("Enter number of resources: ");
	scanf("%d", &num_resources);
	
	// allocate memory for vectors 
	resource = (int *)malloc(num_resources * sizeof(int));
	available = (int *)malloc(num_resources * sizeof(int));
	
	// for each resource, prompt for number of units, set resource and available vectors indices
	printf("Enter number of units for resources (r0 to r%d): ", num_resources-1);
	for(int j = 0; j < num_resources; j++)
	{
	    scanf("%d", &resource[j]);
	    available[j] = resource[j];
	}
	
	// prompt for number of processes
	printf("Enter number of processes: ");
	scanf("%d", &num_processes);
	
	// allocate memory for arrays
	maxclaim = (int *) malloc(num_processes * num_resources * sizeof(int));
	allocated = (int *) malloc(num_processes * num_resources * sizeof(int));
	need = (int *) malloc(num_processes * num_resources * sizeof(int));
	
	// for each process, for each resource, prompt for maximum number of units requested by process, update maxclaim and need arrays
	for (int i = 0; i < num_processes; i++)
	{
	    printf("Enter maximum number of units process p%d will claim from each resource (r0 to r%d): ", i, num_resources-1);
	    for (int j = 0; j < num_resources; j++)
	    {
	        scanf("%d", &maxclaim[i*num_resources + j]);
	        need[i*num_resources + j] = maxclaim[i * num_resources + j];
	    }
	}
	
	// for each process, for each resource, prompt for number of resource units allocated to process 
	for (int i = 0; i < num_processes; i++)
	{
	    printf("Enter number of units of each resource (r0 to r%d) currently allocated to process p%d: ", num_resources-1, i);
	    for(int j = 0; j < num_resources; j++)
	    {
	        scanf("%d", &allocated[i*num_resources + j]);
	        need[i*num_resources + j] -= allocated[i*num_resources + j];
	        available[j] -= allocated[i*num_resources + j];
	        
	    }
	}
	
	// print resource vector, available vector, maxclaim array, allocated array, need array 
	printVector(resource, "Resources");
	printVector(available, "Available");
	printMatrix(maxclaim, "Max Claim");
	printMatrix(allocated, "Allocated");
	printMatrix(need, "Need");
	printf("\n");
	
	return;
}


//**************************************************************
void option2() {
	// declare local variables
	int requestedProcess;
	int requestedResource;
	int requestedUnits;
	
	// prompt for process, resource, and number of units requested
	printf("Enter requesting process: ");
	scanf("%d", &requestedProcess);
	printf("Enter requested resource: ");
	scanf("%d", &requestedResource);
	printf("Enter number of units process p%d is requesting from resource r%d: ", requestedProcess, requestedResource);
	scanf("%d", &requestedUnits);
	
	// if enough units available and request is less than need
	if(available[requestedResource] >= requestedUnits && need[requestedProcess*num_resources + requestedResource] >= requestedUnits)
	{
	    // reduce number of available units
	    available[requestedResource] -= requestedUnits;
	    
		// increase number of allocated units
		allocated[requestedProcess*num_resources + requestedResource] += requestedUnits;
		
		// reduce number of needed units
		need[requestedProcess*num_resources + requestedResource] -= requestedUnits;
		
		// print updated available, allocated, and need vectors/matrices
	    printVector(available, "Available");
    	printMatrix(allocated, "Allocated");
    	printMatrix(need, "Need");
    	printf("\n");
	}
    else
    {
        printf("Request Denied\n");
    }
    
	return;
}


//**************************************************************
void option3() {
	// declare local variables
	int releasedProcess;
	int releasedResource;
	int releasedUnits;
	
	// prompt for process, resource, and number of units requested
	printf("Enter releasing process: ");
	scanf("%d", &releasedProcess);
	printf("Enter released resource: ");
	scanf("%d", &releasedResource);
	printf("Enter number of units process p%d is releasing from resource r%d: ", releasedProcess, releasedResource);
	scanf("%d", &releasedUnits);

	// if enough units allocated
	if(allocated[releasedProcess*num_resources + releasedResource] >= releasedUnits)
	{
	    // increase number of available units
	    available[releasedResource] += releasedUnits;
	    
		// reduce number of allocated units
		allocated[releasedProcess*num_resources + releasedResource] -= releasedUnits;
		
		// increase number of needed units
		need[releasedProcess*num_resources + releasedResource] += releasedUnits;
		
		// print updated available, allocated, and need vectors/matrices
		printVector(available, "Available");
    	printMatrix(allocated, "Allocated");
    	printMatrix(need, "Need");
    	printf("\n");
	}
	else
    {
        printf("Release cannot be performed\n");
    }
    
	return;
}


//******************************************************************
void option4() {
	// declare local variables
	int num_done = 0;
	int at_least_one = 1;
	int *processedOrder = (int *) malloc(num_processes * sizeof(int));;
	int *isProcessed = (int *) calloc(num_processes, sizeof(int));
	int safe;

	
	// while not all processes are processed
	while((num_done < num_processes) && (at_least_one))
	{
	    at_least_one = 0;
	    // for each process  
	    for(int i = 0; i < num_processes; i++)
	    {
            safe = 1;
	        // if process has not been processed yet 
	        if(isProcessed[i] == 0)
	        {
	            // print message comparing need vector with available vector
	            printf("Comparing: < ");
	            for(int j = 0; j < num_resources; j++)
	            {
	                printf("%d ", need[i*num_resources + j]);
	            }
	            printf("> <= < ");
	            for(int j = 0; j < num_resources; j++)
	            {
	                printf("%d ", available[j]);
	                
	                // check for safe processing by comparing process' need vector to available vector
	                if(need[i*num_resources + j] > available[j])
	                {
	                    safe = 0;
	                }
	            }
	            printf("> : Process p%d ", i);
	            
	            // if each resource is available 
	            if(safe == 1)
                {
                    // print message that process can be processed
                    printf("can be sequenced \n");
                    
			        // update number of available units of resource 
			        // for each resource 
			        for(int j = 0; j < num_resources; j++)
			        {
			            available[j] += allocated[i*num_resources + j];
			            allocated[i*num_resources + j] = 0;
			            need[i*num_resources + j] = maxclaim[i*num_resources + j];
			        }
			        
				    // increment number of sequenced processes 
				    isProcessed[i] = 1;
				    processedOrder[num_done] = i;
				    num_done++;
				    at_least_one = 1;
				    	
	            }
                else
                {
                    printf("cannot be sequenced \n");
	            }
	        }
	    }
	    
	    //if (no process was processed in the final round of the for-loop)
	    
	}
	
	if(at_least_one == 0)
	{
	    // print message of deadlock among processes not processed
	    printf("Deadlock among processes: ");
	    for(int i = 0; i < num_processes; i ++)
	    {
	        if(isProcessed[i] == 0)
	        {
	            printf("p%d ",i);
	        }
	    }
	}
	else
	{
	    //print safe sequence of processes
	    printf("Safe sequence of processes: ");
    	for(int i = 0; i< num_processes; i++)
    	{
    	    printf("p%d ", processedOrder[i]);
    	}
	}
	
    printf("\n\n");
    free(processedOrder);
	free(isProcessed);
	
	return;
}


//******************************************************************
void option5() {
	// check if vectors/array are not NULL--if so, free each vector/array 	);
	if(resource != NULL)
	{
	    free(resource);
	}
	if(available != NULL)
	{
	    free(available);
	}
	if(maxclaim != NULL)
	{
	    free(maxclaim);
	}
	if(allocated != NULL)
	{
	    free(allocated);
	}
	if(need != NULL)
	{
	    free(need);
	}
	
	return;
}


//*************************************************************
int main() {
    // declare local vars 
	int choice = -1;
	while(choice != 5)
    {
        /* print menu of options */
        printf("Banker's Algorithm\n");
        printf("------------------\n");
        printf("1) Enter claim graph\n");
        printf("2) Request resource\n");
        printf("3) Release resource\n");
        printf("4) Determine safe sequence\n");
        printf("5) Quit program\n");
        
        // prompt for menu selection 
        printf("\nEnter selection: ");
        scanf("%d", &choice);
        
        printf("\n");
        
        /* call appropriate procedure based on choice--use switch statement or series of if, else if, else statements */
        switch(choice)
        {
            case 1: 
                option1();
                break;
            case 2:
                option2();
                break;
            case 3:
                option3();
                break;
            case 4:
                option4();
                break;
            case 5:
                option5();
        }
        
    }
    
    
   printf("\nQuitting program...");

	 return 1; // indicates success 
} // end of procedure 




