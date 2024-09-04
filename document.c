/*****************/
/* Mihir M Celly */
/* 118647845     */
/* mcelly        */
/*****************/

#include "document.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

/* Static function to replace text within a line */
static void replace_in_line(char *line, const char *target,                    
    const char *replacement);


/*******************************************/
/* Initialize a document with a given name */
/*******************************************/
int init_document(Document *doc, const char *name) {
    if (doc == NULL || name == NULL) {
        return FAILURE;
    }

    if (strlen(name) >= MAX_STR_SIZE) {
        return FAILURE;
    }

    strcpy(doc->name, name);
    doc->number_of_paragraphs = 0;

    return SUCCESS;
}


/***********************************/
/* Reset the content of a document */
/***********************************/
int reset_document(Document *doc) {
    if (doc == NULL) {
        return FAILURE;
    }

    doc->number_of_paragraphs = 0;

    return SUCCESS;
}

/***********************************/
/* Print the content of a document */
/***********************************/
int print_document(Document *doc) {
    int i, j;

    if (doc == NULL) {
        return FAILURE;
    }

    printf("Document name: \"%s\"\n", doc->name);
    printf("Number of Paragraphs: %d\n", doc->number_of_paragraphs);

    for (i = 0; i < doc->number_of_paragraphs; i++) {
        for (j = 0; j < doc->paragraphs[i].number_of_lines; j++) {
            if (doc->paragraphs[i].lines[j][0] == '\0') {
                break;
            }
            printf("%s\n", doc->paragraphs[i].lines[j]);
        }
        if (i < (doc->number_of_paragraphs - 1)) {
            printf("\n");
        }
    }

    return SUCCESS;
}

/************************************************************/
/* Add a new paragraph after the specified paragraph number */
/************************************************************/
int add_paragraph_after(Document *doc, int paragraph_number) {
    int i;

    if (doc == NULL) {
        return FAILURE;
    }

    if (doc->number_of_paragraphs >= MAX_PARAGRAPHS || paragraph_number > 
        doc->number_of_paragraphs) {
        return FAILURE;
    }

    /* Shift existing paragraphs down to make space for the new paragraph */
    for (i = doc->number_of_paragraphs - 1; i >= paragraph_number; i--) {
        doc->paragraphs[i + 1] = doc->paragraphs[i];
    }

    /* Initialize the new paragraph */
    doc->paragraphs[paragraph_number].number_of_lines = 0;

    /* Increment the number of paragraphs */
    (doc->number_of_paragraphs)++;

    return SUCCESS;
}

/**********************************************************/
/* Add a new line after the specified line in a paragraph */
/**********************************************************/
int add_line_after(Document *doc, int paragraph_number, int line_number, 
    const char *new_line) {

    int num_lines, i;

    /* Check for NULL pointers and invalid line content */
    if (doc == NULL || new_line == NULL) {
        return FAILURE;
    }

    /* Check if paragraph number is valid */
    if (paragraph_number <= 0 || paragraph_number > doc->number_of_paragraphs 
        || doc->number_of_paragraphs >= MAX_PARAGRAPHS) {
        return FAILURE;
    }

    /* Check if line number is valid */
    if (line_number < 0 || line_number > doc->paragraphs[paragraph_number - 1].
        number_of_lines || doc->paragraphs[paragraph_number - 1].number_of_lines   
        >= MAX_PARAGRAPH_LINES) {

        return FAILURE;
    }

    num_lines = doc->paragraphs[paragraph_number - 1].number_of_lines;

    /* Shift lines to make space for the new line */
    for (i = num_lines; i > line_number; i--) {
        strcpy(doc->paragraphs[paragraph_number - 1].lines[i], doc->paragraphs[
                paragraph_number - 1].lines[i - 1]);
    }

    /* Add the new line */
    strcpy(doc->paragraphs[paragraph_number - 1].lines[line_number], new_line);
    /* Increment number_of_lines */
    doc->paragraphs[paragraph_number - 1].number_of_lines++; 

    return SUCCESS;
}

/****************************************************/
/* Get the number of lines in a specified paragraph */
/****************************************************/
int get_number_lines_paragraph(Document *doc, int paragraph_number, 
    int *number_of_lines) {
    /* Check for NULL pointers */
    if (doc == NULL || number_of_lines == NULL) {
        return FAILURE;
    }

    /* Check if paragraph number is valid */
    if (paragraph_number < 0 || paragraph_number >=    
        doc->number_of_paragraphs) {
        return FAILURE;
    }

    /* Retrieve the number of lines in the specified paragraph */
    *number_of_lines = doc->paragraphs[paragraph_number].number_of_lines;

    return SUCCESS;
}

/**********************************************/
/* Append a new line to a specified paragraph */
/**********************************************/
int append_line(Document *doc, int paragraph_number, const char *new_line) {
    int num_lines = 0;

    if (doc == NULL || new_line == NULL) {
        return FAILURE;
    }

    num_lines = doc->paragraphs[paragraph_number - 1].number_of_lines;

    if (paragraph_number > MAX_PARAGRAPHS || num_lines > MAX_PARAGRAPH_LINES) {
        return FAILURE;
    }

    add_line_after(doc, paragraph_number, num_lines, new_line);

    return SUCCESS;
}

/********************************************/
/* Remove a line from a specified paragraph */
/********************************************/
int remove_line(Document *doc, int paragraph_number, int line_number) {
    int i;

    /* Check for NULL pointer */
    if (doc == NULL) {
        return FAILURE;
    }

    /* Adjust line_number to match zero-based indexing */
    line_number--;

    /* Check if paragraph number is valid */
    if (paragraph_number <= 0 || paragraph_number > doc->number_of_paragraphs) {
        return FAILURE;
    }

    /* Check if line number is valid */
    if (line_number < 0 || line_number >= doc->paragraphs[paragraph_number - 1].
        number_of_lines) {

        return FAILURE;
    }

    /* Remove the specified line by shifting subsequent lines */
    for (i = line_number; i < doc->paragraphs[paragraph_number - 1].
        number_of_lines - 1; i++) {
        strcpy(doc->paragraphs[paragraph_number - 1].lines[i], doc->paragraphs[
                paragraph_number - 1].lines[i + 1]);
    }

    /* Clear the last line */
    doc->paragraphs[paragraph_number - 1].lines[i][0] = '\0';

    /* Decrement the number of lines in the paragraph */
    doc->paragraphs[paragraph_number - 1].number_of_lines--;

    return SUCCESS;
}

/********************************************/
/* Load a document from an array of strings */
/********************************************/
int load_document(Document *doc, char data[][MAX_STR_SIZE + 1], 
    int data_lines) {
    int i, j = 1;

    /* Check for NULL pointers and ensure data_lines is valid */
    if (doc == NULL || data == NULL || data_lines == 0) {
        return FAILURE;
    }

    add_paragraph_after(doc, 0);

    /* Add paragraphs and lines from the data array to the document */
    for (i = 0; i < data_lines; i++) {
        if (strcmp(data[i], "") == 0) {
            /* Add a new paragraph when an empty string is encountered */
            add_paragraph_after(doc, j);
            j++;
        } else {
            append_line(doc, j, data[i]);
        }
    }

    return SUCCESS;
}

/****************************************************/
/* Auxiliary function to replace text within a line */
/****************************************************/
static void replace_in_line(char *line, const char *target, const 
    char *replacement) {

    char *pos = line;  /* Pointer to track the position within the line */
    int target_len, replacement_len; /* target and replacement string length */

    /* Check for NULL pointers */
    if (line == NULL || target == NULL || replacement == NULL) {
        return;  /* Exit the function if any of the pointers are NULL to */
                 /* prevent segmentation faults                          */
    }

    /* Calculate the length of the replacement string */
    replacement_len = strlen(replacement);  

    /* Calculate the length of the target string */
    target_len = strlen(target);  

    /* Iterate through the line and replace occurrences of the target string */
    while ((pos = strstr(pos, target)) != NULL) {
        /* Move the rest of the line to accommodate the replacement */
        memmove(pos + replacement_len, pos + target_len, 
            strlen(pos + target_len) + 1);
        /* Copy the replacement string into the position of the target */
        memcpy(pos, replacement, replacement_len);
        /* Move the position to the end of the replacement */
        pos += replacement_len;
    }
}


/***********************************************/
/* Replace occurrences of a target string with */
/* a replacement string in the document        */
/***********************************************/
int replace_text(Document *doc, const char *target, const char *replacement) {
    int i, j;

    /* Check for NULL pointers */
    if (doc == NULL || target == NULL || replacement == NULL) {
        return FAILURE;
    }

    /* Iterate through all paragraphs and lines */
    for (i = 0; i < doc->number_of_paragraphs; i++) {
        for (j = 0; j < doc->paragraphs[i].number_of_lines; j++) {
            /* Call auxiliary function to replace text within a line */
            replace_in_line(doc->paragraphs[i].lines[j], target, replacement);
        }
    }

    return SUCCESS;
}

/************************************************************/
/* Highlight occurrences of a target string in the document */
/************************************************************/
int highlight_text(Document *doc, const char *target) {
    int i, j;
    size_t target_len;

    /* Check for NULL pointers */
    if (doc == NULL || target == NULL) {
        return FAILURE;
    }

    /* Get the length of the target string */
    target_len = strlen(target);

    /* Iterate over each paragraph */
    for (i = 0; i < doc->number_of_paragraphs; i++) {
        /* Iterate over each line in the paragraph */
        for (j = 0; j < doc->paragraphs[i].number_of_lines; j++) {
            /* Pointer to the current position and content of the line */
            char *content = doc->paragraphs[i].lines[j], *position = content;

            /* Find the first occurrence of the target in the line */
            while ((position = strstr(position, target)) != NULL) {
                /* Insert HIGHLIGHT_START_STR before the target */
                memmove(position + strlen(HIGHLIGHT_START_STR), position, 
                    strlen(position) + 1);
                memcpy(position, HIGHLIGHT_START_STR, 
                    strlen(HIGHLIGHT_START_STR));

                /* Move the position to the end of the inserted */
                /* HIGHLIGHT_START_STR and the target */
                position += target_len + strlen(HIGHLIGHT_START_STR);

                /* Insert HIGHLIGHT_END_STR after the target */
                memmove(position + strlen(HIGHLIGHT_END_STR), position, 
                    strlen(position) + 1);
                memcpy(position, HIGHLIGHT_END_STR, strlen(HIGHLIGHT_END_STR));

                /* Move the position to the end of the */
                /* inserted HIGHLIGHT_END_STR */
                position += strlen(HIGHLIGHT_END_STR);
            }
        }
    }

    return SUCCESS;
}

/**********************************/
/* Remove occurrences of a target */
/* string from the document       */
/**********************************/
int remove_text(Document *doc, const char *target) {
    int i, j;

    /* Check for NULL pointers */
    if (doc == NULL || target == NULL) {
        return FAILURE;
    }

    /* Iterate through all paragraphs and lines */
    for (i = 0; i < doc->number_of_paragraphs; i++) {
        for (j = 0; j < doc->paragraphs[i].number_of_lines; j++) {
            /* Find and remove the target text from each line */
            char *pos;
            while ((pos = strstr(doc->paragraphs[i].lines[j], target)) != NULL) {
                memmove(pos, pos + strlen(target), strlen(pos + strlen(target)) + 1);
            }
        }
    }

    return SUCCESS;
}

/**************************************/
/* Load a document from a text file   */
/**************************************/
int load_file(Document *doc, const char *filename) {
    FILE *file;
    char lines[MAX_STR_SIZE + 1], line1, line2[MAX_STR_SIZE];
    int paragraph_number = 1, num_values;

    if (doc == NULL || filename == NULL) {
        return FAILURE;
    }

    file = fopen(filename, "r");

    if (file == NULL) {
        return FAILURE;
    }

    add_paragraph_after(doc, 0);

    while(fgets(lines, MAX_STR_SIZE + 1, file)) {
        
        char *newline = strchr(lines, '\n');\
        
        if (newline) {
            *newline = '\0';
        }

        num_values = sscanf(lines, " %c%s", &line1, line2);

        if (line1 != '#' && num_values < 2) {
            add_paragraph_after(doc, paragraph_number);
            paragraph_number++;
        }
        else if (line1 != '#' && num_values >= 2) {
            append_line(doc, paragraph_number, lines);
        }
    }

    fclose(file);
    return SUCCESS;
}

int save_document(Document *doc, const char *filename) {
    FILE *file;
    int i, j;

    /* Check for NULL pointers */
    if (doc == NULL || filename == NULL) {
        return FAILURE;
    }

    /* Open the file for writing */
    file = fopen(filename, "w");
    if (file == NULL) {
        return FAILURE; /* Failed to open file */
    }

    /* Write each paragraph to the file */
    for (i = 0; i < doc->number_of_paragraphs; i++) {
        for (j = 0; j < doc->paragraphs[i].number_of_lines; j++) {
            /* Write each line followed by a newline character */
            fprintf(file, "%s\n", doc->paragraphs[i].lines[j]);
        }
        /* Separate paragraphs by a newline character */
        if (i < doc->number_of_paragraphs - 1) {
            fprintf(file, "\n");
        }
    }

    /* Close the file */
    fclose(file);

    return SUCCESS;
}