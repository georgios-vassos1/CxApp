#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

int main() {
    const char *filename = "/Users/gva/Desktop/CHW/src/HW2/data/reviews.txt";
    review_t *review;

    // Read reviews from the file
    while ((review = read_review(filename)) != NULL) {
        // Print the review details
        printf("Movie Name: %s\n", review->movie_name);
        printf("Reviewer Name: %s\n", review->reviewer_name);
        printf("Review Text: %s\n", review->review_text);
        printf("Review Score: %.1f\n\n", review->review_score);

        // Free the dynamically allocated memory
        free(review->movie_name);
        free(review->reviewer_name);
        free(review->review_text);
        free(review);
    }

    return 0;
}
