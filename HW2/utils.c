#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

// Helper function to trim leading and trailing whitespace and quotes
char *trim(char *str) {
    char *end;

    // Trim leading spaces and quotes
    while (*str == ' ' || *str == '"') str++;

    // Trim trailing spaces and quotes
    end = str + strlen(str) - 1;
    while (end > str && (*end == ' ' || *end == '"')) end--;

    // Write new null terminator
    *(end + 1) = '\0';

    return str;
}

review_t* read_review(const char *filename) {
    static FILE *file = NULL;
    char line[1024];

    if (file == NULL) {
        file = fopen(filename, "r");
        if (file == NULL) {
            perror("Failed to open file");
            return NULL;
        }
    }

    if (fgets(line, sizeof(line), file) == NULL) {
        fclose(file);
        file = NULL;
        return NULL;
    }

    // Allocate memory for the review
    review_t *review = (review_t *)malloc(sizeof(review_t));
    if (review == NULL) {
        perror("Failed to allocate memory");
        fclose(file);
        file = NULL;
        return NULL;
    }

    // Parse the line and fill in the review structure
    char *token;

    // Movie name
    token = strtok(line, ",");
    if (token == NULL) {
        free(review);
        fclose(file);
        file = NULL;
        return NULL;
    }
    review->movie_name = strdup(trim(token));

    // Reviewer name
    token = strtok(NULL, ",");
    if (token == NULL) {
        free(review->movie_name);
        free(review);
        fclose(file);
        file = NULL;
        return NULL;
    }
    review->reviewer_name = strdup(trim(token));

    // Review text
    token = strtok(NULL, ",");
    if (token == NULL) {
        free(review->movie_name);
        free(review->reviewer_name);
        free(review);
        fclose(file);
        file = NULL;
        return NULL;
    }
    review->review_text = strdup(trim(token));

    // Review score
    token = strtok(NULL, ",");
    if (token == NULL) {
        free(review->movie_name);
        free(review->reviewer_name);
        free(review->review_text);
        free(review);
        fclose(file);
        file = NULL;
        return NULL;
    }
    review->review_score = atof(trim(token));

    return review;
}
