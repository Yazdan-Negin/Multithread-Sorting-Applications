#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX_SIZE 100

int global_array[MAX_SIZE];
int sorted_array[MAX_SIZE];
int array_size;

void bubble_sort(int *arr, int n) {
    int i, j, temp;
    for (i = 0; i < n-1; i++) {
        for (j = 0; j < n-i-1; j++) {
            if (arr[j] > arr[j+1]) {
                temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
        }
    }
}

void merge(int *arr1, int n1, int *arr2, int n2, int *result) {
    int i = 0, j = 0, k = 0;
    
    while (i < n1 && j < n2) {
        if (arr1[i] <= arr2[j]) {
            result[k++] = arr1[i++];
        } else {
            result[k++] = arr2[j++];
        }
    }
    
    while (i < n1) {
        result[k++] = arr1[i++];
    }
    
    while (j < n2) {
        result[k++] = arr2[j++];
    }
}

void *sort_first_half(void *arg) {
    int half = array_size / 2;
    bubble_sort(global_array, half);
    pthread_exit(NULL);
}

void *sort_second_half(void *arg) {
    int half = array_size / 2;
    bubble_sort(global_array + half, array_size - half);
    pthread_exit(NULL);
}

void *merge_sorted_halves(void *arg) {
    int half = array_size / 2;
    merge(global_array, half, global_array + half, array_size - half, sorted_array);
    pthread_exit(NULL);
}

int main() {
    pthread_t sort_thread1, sort_thread2, merge_thread;
    int i;
    
    printf("Enter the number of elements (max %d): ", MAX_SIZE);
    scanf("%d", &array_size);
    
    if (array_size > MAX_SIZE || array_size <= 0) {
        printf("Invalid array size!\n");
        return 1;
    }
    
    printf("Enter %d integers:\n", array_size);
    for (i = 0; i < array_size; i++) {
        scanf("%d", &global_array[i]);
    }
    
    pthread_create(&sort_thread1, NULL, sort_first_half, NULL);
    pthread_create(&sort_thread2, NULL, sort_second_half, NULL);
    
    pthread_join(sort_thread1, NULL);
    pthread_join(sort_thread2, NULL);
    
    pthread_create(&merge_thread, NULL, merge_sorted_halves, NULL);
    
    pthread_join(merge_thread, NULL);
    
    printf("Sorted array:\n");
    for (i = 0; i < array_size; i++) {
        printf("%d ", sorted_array[i]);
    }
    printf("\n");
    
    return 0;
}