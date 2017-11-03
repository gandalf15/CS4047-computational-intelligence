#include "defuzzifier.h"
#include <stdlib.h>

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

double* areaAndCentroid(double signal, int *tuple, double min_x) {
    double a, b, c, x1, x2;
    double *results = malloc(sizeof(double)*2);
    x1 = (signal + (tuple[0]/tuple[2]) -1) * tuple[2];
    x2 = (-signal + (tuple[0]/tuple[2]) +1) * tuple[3];
    a = x2-x1;
    b = abs(tuple[0]-tuple[1]);
    c = tuple[2];
    // calculate the area
    results[0] = ((a+b)/2)*signal;
    //calculate the centroid
    results[1] = ((2*a*c)+(a*a)+(c*b)+(a*b)+(b*b))/3*(a+b);
    // convert the number to graph x coordinates
    results[1] = results[1] - min_x;
    return results;
}

double defuzzify (set_signal *setSignalTable, var_sets **dptrVarTable) {
    double crisp_value = 0;
    set_signal *ptrSetSignal = setSignalTable;
    var_sets *variableSets = findVarSets(dptrVarTable, ptrSetSignal->var_name);
    fuzzy_set *fuzzySet;
    int num_of_sets = variableSets->number_of_sets;
    // allocate enough memmory to save pointers to area values and centroids
    double **areasAndCentroids = malloc(sizeof(double*)*num_of_sets);
    // calculate area for each value
    // init to max value of int
    int min_value_graph_x = ((unsigned)-1)>>1;
    int *tuple;
    for (int i=0; i <= num_of_sets; ++i) {
        fuzzySet = findFuzzySet( &(variableSets->sets_table), ptrSetSignal->val_name );
        tuple = fuzzySet->tuple;
        min_value_graph_x = MIN( min_value_graph_x, ( tuple[0] - tuple[2] ) );
        ptrSetSignal = ptrSetSignal->hh.next;
    }
    // get the head of list again
    ptrSetSignal = setSignalTable;
    double centre_A, centre_B;
    for (int i=0; i <= num_of_sets; ++i) {
        fuzzySet = findFuzzySet( &(variableSets->sets_table), ptrSetSignal->val_name );
        tuple = fuzzySet->tuple;
        areasAndCentroids[i] = areaAndCentroid ( ptrSetSignal->out_signal, tuple, min_value_graph_x);
        centre_A += areasAndCentroids[i][0] * areasAndCentroids[i][1];
        centre_B += areasAndCentroids[i][0];
        ptrSetSignal = ptrSetSignal->hh.next;
    }

    return crisp_value;
}