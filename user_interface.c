/*****************/
/* Mihir M Celly */
/* 118647845     */
/* mcelly        */
/*****************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>
#include <ctype.h>
#include "document.h"

static char *remquotes_helperfn(char *command, char *result);
static int is_valid_command(Document *doc, char *command);
static char find_first_non_space(char* str);

/***************************************/
/* Function to add a paragraph after a */ 
/* specified command in a document.    */
/***************************************/
int add_paragraph_after_command(Document *doc, char *command) {

    char name[MAX_STR_SIZE + 1], extra[MAX_STR_SIZE + 1];
    int num, num_values;

    /* Parsing the command to extract relevant information */
    num_values = sscanf(command, "%s%d%s", name, &num, extra);

    /* If parsing fails or number is negative, return failure */
    if (num_values != 2 || num < 0) {
        return FAILURE;
    }
    
    /* Shift existing paragraphs down to make space for the new paragraph */
    if (add_paragraph_after(doc, num)) {
        printf("add_paragraph_after failed\n");
    }

    /* Return SUCCESS if the paragraph insertion was successful */
    return SUCCESS;
}

/**************************************************/
/* This function parses a command string to add a */ 
/* line after a specified position in a document. */
/**************************************************/
int add_line_after_command(Document *doc, char *command) {

    char name[MAX_STR_SIZE + 1], line[MAX_STR_SIZE + 1], buffer[MAX_STR_SIZE + 1];
    int num_values, num1, num2;

    /* Parse the command string and extract relevant information */
    num_values = sscanf(command, "%s%d%d%s", name, &num1, &num2, buffer);

    /* Check if the parsing was successful and if the extracted values are valid */
    if (num_values != 4 || num1 <= 0 || num2 < 0) {

        /* If the command is malformed or the parameters are invalid, return FAILURE */
        return FAILURE;

    }

    /* Check if the command contains an asterisk (*) character */
    if (!strchr(command, '*')) {

        /* If the command does not contain an asterisk, it's invalid, so return FAILURE */
        return FAILURE;
    
    }

    /* Extract the line to be added from the command string */
	strcpy(line, strchr(command, '*') + 1);
	
    /* Call the add_line_after function to insert the line after the specified position */
	if (add_line_after(doc, num1, num2, line)) {

        /* If the add_line_after function fails, print an error message */
	    printf("add_line_after failed\n");

	}
    
    /* Return SUCCESS if the line insertion was successful */
    return SUCCESS;

}

/********************************************/
/* This function processes a command string */
/* to print the contents of a document.     */
/********************************************/
int print_document_command(Document *doc, char *command) {

    char name[MAX_STR_SIZE + 1], buffer[MAX_STR_SIZE + 1];
    int num_values;

    /* Parse the command string and extract relevant information */
    num_values = sscanf(command, "%s%s", name, buffer);

    /* Check if the document pointer is valid*/
    if (!doc) {

        /* If the document pointer is NULL, return FAILURE */
        return FAILURE;

    }

    /* Check if the parsing of the command string was successful */
    if (num_values != 1) {

        /* If the command string is malformed, return FAILURE */
        return FAILURE;

    }
    else {

        /* If the command string is well-formed, print the document */
        if (print_document(doc)) {

            /* If printing the document fails, print an error message */
	        printf("print_document failed\n");

        }

        /* Return SUCCESS if the document is printed successfully */
        return SUCCESS;

    }
}

/******************************************************/
/* This function processes a command string to append */
/* a line to a document after a specified position.   */
/******************************************************/
int append_line_command(Document *doc, char *command) {

    char name[MAX_STR_SIZE + 1], line[MAX_STR_SIZE + 1], buffer[MAX_STR_SIZE + 1];
    int num_values, num;

    /* Parse the command string and extract relevant information */
    num_values = sscanf(command, "%s%d%s", name, &num, buffer);

    /* Check if the parsing was successful and if the extracted values are valid */
    if (num_values != 3 || num <= 0) {

        /* If the command is malformed or the parameters are invalid, return FAILURE */
        return FAILURE;

    }
    
    /* Check if the command contains an asterisk (*) character */
    if (!strchr(command, '*')) {

        /* If the command does not contain an asterisk, it's invalid, so return FAILURE */
        return FAILURE;

    }

    /* Extract the line to be appended from the command string */
    strcpy(line, strchr(command, '*') + 1);
    
    /* Call the append_line function to append the line after the specified position */
	if (append_line(doc, num, line)) {

        /* If the append_line function fails, print an error message */
        printf("append_line failed\n");

	}
    
    /* Return SUCCESS if the line is appended successfully */
    return SUCCESS;

}

/******************************************************/
/* This function processes a command string to remove */
/* lines from a document within a specified range.    */
/******************************************************/
int remove_line_command(Document *doc, char *command) {

    char name[MAX_STR_SIZE + 1], buffer[MAX_STR_SIZE + 1];
    int num_values, num1, num2;

    /* Parse the command string and extract relevant information */
    num_values = sscanf(command, "%s%d%d%s", name, &num1, &num2, buffer);

    /* Check if the parsing was successful and if the extracted values are valid */
    if (num_values != 3 || num1 <= 0 || num2 <= 0) {

        /* If the command is malformed or the parameters are invalid, return FAILURE */
        return FAILURE;

    }

    /* Call the remove_line function to remove lines within the specified range */
    if (remove_line(doc, num1, num2)) {

        /* If the remove_line function fails, print an error message */
	    printf("remove_line failed\n");

    }

    /* Return SUCCESS if the lines are removed successfully */
    return SUCCESS;

}

/***********************************************/
/* This function processes a command string to */
/* load the contents of a file into a document.*/
/***********************************************/
int load_file_command(Document *doc, char *command) {

    char file[MAX_STR_SIZE + 1], name[MAX_STR_SIZE + 1], buffer[MAX_STR_SIZE + 1];
    int num_values;

    /* Parse the command string and extract relevant information */
    num_values = sscanf(command, "%s%s%s", name, file, buffer);

    /* Check if the parsing was successful and if the extracted values are valid */
    if (num_values != 2) {

        /* If the command is malformed, return FAILURE */
        return FAILURE;

    }

    /* Call the load_file function to load the contents of the file into the document */
    if (load_file(doc, file)) {

        /* If the load_file function fails, print an error message */
        printf("load_file failed\n");

    }
    
    /* Return SUCCESS if the file is loaded successfully */
    return SUCCESS;
}

/********************************************************/
/* This function processes a command string to replace  */ 
/* a target text with a replacement text in a document. */
/********************************************************/
int replace_text_command(Document *doc, char *command) {
    char name[MAX_STR_SIZE + 1], target[MAX_STR_SIZE + 1], replace[MAX_STR_SIZE + 1], 
    *str, *str2 = NULL, *str3 = NULL, *str4 = NULL;
    int num_values;

    /* Parse the command string and extract relevant information */
    num_values = sscanf(command, "%s%s%s", name, target, replace);
    
    /* Check if the parsing was successful and if the extracted values are valid */
    if (num_values != 3) {

        /* If the command is malformed, return FAILURE */
        return FAILURE;

    }

    /* Find the positions of the quotation marks in the command string*/
    str = strstr(command, "\"");
    str2 = strstr(str + 1, "\"");
    str3 = strstr(str2 + 1, "\"");
    str4 = strstr(str3 + 1, "\"");
    
    /* Check if all quotation marks were found */
    if (!str || !str2 || !str3 || !str4) {    

        /* If any quotation marks are missing, return FAILURE */   
        return FAILURE;

    }

    /* Extract the target and replacement strings from the command string */
    strncpy(target, str + 1, str2 - str);
    target[str2 - (str + 1)] = '\0';
    strncpy(replace, str3 + 1, str4 - str3);
    replace[str4 - (str3 + 1)] = '\0';

    /* Call the replace_text function to replace the target text with the replacement text */
    if (replace_text(doc, target, replace)) {

        /* If the replace_text function fails, print an error message */
        printf("replace_text failed\n");

    }

    /* Return SUCCESS if the text replacement is successful */
    return SUCCESS;
}

/********************************************/
/* This function processes a command string */
/* to highlight text in a document.         */
/********************************************/
int highlight_text_command(Document *doc, char *command) {

    char name[MAX_STR_SIZE + 1], target[MAX_STR_SIZE + 1], 
    temp[MAX_STR_SIZE + 1], *str, *result;
    int num_values;
    
    /* Parse the command string and extract relevant information */
    num_values = sscanf(command, "%s%s", name, temp);
    str = strstr(command, "\"");

    /* Check if the parsing was successful and if the necessary characters are present in the command string */
    if (num_values != 2 || str == NULL) {

        /* If the command is malformed or if quotation marks are missing, return FAILURE */
        return FAILURE;

    }

    /* Call the remquotes_helperfn function to remove quotation marks from the target text */
    result = remquotes_helperfn(str, target);

    /* Check if the result of removing quotation marks is successful */
    if (!result) {

        /* If removing quotation marks fails, return FAILURE */
        return FAILURE;

    }

    /* Call the highlight_text function to highlight the target text in the document */
    highlight_text(doc, result);

    /* Return SUCCESS if the text is successfully highlighted */
	return SUCCESS;
}

/********************************************/
/* This function processes a command string */
/* to remove text from a document.          */
/********************************************/
int remove_text_command(Document *doc, char *command) {

    char name[MAX_STR_SIZE + 1], target[MAX_STR_SIZE + 1], 
    temp[MAX_STR_SIZE + 1], *str, *result;
    int num_values;
    
    /* Parse the command string and extract relevant information */
    num_values = sscanf(command, "%s%s", name, temp);
    str = strstr(command, "\"");

    /* Check if the parsing was successful and if the necessary characters are present in the command string */
    if (num_values != 2 || str == NULL) {

        /* If the command is malformed or if quotation marks are missing, return FAILURE */
        return FAILURE;

    }

    /* Call the remquotes_helperfn function to remove quotation marks from the target text */
    result = remquotes_helperfn(str, target);

    /* Check if the result of removing quotation marks is successful */
    if (!result) {

        /* If removing quotation marks fails, return FAILURE */
        return FAILURE;

    }

    /* Call the remove_text function to remove the target text from the document */
    remove_text(doc, result);

    /* Return SUCCESS if the text is successfully removed */
	return SUCCESS;
}

/**************************************************************************/
/* This function processes a command string to save a document to a file. */
/**************************************************************************/
int save_document_command(Document *doc, char *command) {

    char name[MAX_STR_SIZE + 1], file[MAX_STR_SIZE + 1], 
    buffer[MAX_STR_SIZE + 1];
    int num_values;

    /* Parse the command string and extract relevant information */
    num_values = sscanf(command, "%s%s%s", name, file, buffer);

    /* Check if the parsing was successful and if the extracted values are valid */
    if (num_values != 2) {

        /* If the command is malformed, return FAILURE */
        return FAILURE;

    }

    /* Call the save_document function to save the document to the file */
    if (save_document(doc, file)) {

        /* If the save_document function fails, print an error message */
	    printf("save_document failed\n");

    }      
    
    /* Return SUCCESS if the document is saved successfully */
    return SUCCESS;
}

/*****************************************************************/
/* This function processes a command string to reset a document. */
/*****************************************************************/
int reset_document_command(Document *doc, char *command) {

    char name[MAX_STR_SIZE + 1], buffer[MAX_STR_SIZE + 1];
    int num_values;

    /* Parse the command string and extract relevant information */
    num_values = sscanf(command, "%s%s", name, buffer);

    /* Check if the parsing was successful and if the extracted values are valid */
    if (num_values != 1) {

        /* If the command is malformed, return FAILURE */
        return FAILURE;

    }

    /* Call the reset_document function to reset the document */
    reset_document(doc);

    /* Return SUCCESS if the document is reset successfully */
    return SUCCESS;
}

/*************************************************/
/* This static helper function removes quotation */
/* marks from a string and returns the result.   */
/*************************************************/
static char *remquotes_helperfn(char *command, char *result) {

    char *str, *str2;

    /* Find the first occurrence of a quotation mark in the string */
    str = strstr(command, "\"");

    /* Check if a quotation mark was found */
    if (!str) {

        /* If no quotation mark is found, return NULL */
        return NULL;

    }
    else {

        /* Find the next occurrence of a quotation mark after the first one */
        str2 = strstr(str + 1, "\"");

        /* Check if a second quotation mark was found */
        if (!str2) {
            
            /* If no second quotation mark is found, return NULL */
            return NULL;

        }
        else {

            /* Copy the substring between the two quotation marks to the result array */
            strncpy(result, str + 1, str2 - str);
            
            /* Add a null terminator to the result array */
	        result[str2 - (str + 1)] = '\0';

            /* Return a pointer to the modified string */
	        return result;

        }
    }
}

/***********************************************************************/
/* This function validates and executes commands based on their names. */
/***********************************************************************/
static int is_valid_command(Document *doc, char *command) {

    int valid = FAILURE;

    /* Check if the command contains the substring "add_paragraph_after" */
    if (strstr(command, "add_paragraph_after")) {

        /* If it does, attempt to execute the corresponding command*/
        if (!add_paragraph_after_command(doc, command)) {

            /* If the command is successful, mark it as valid*/
	        valid = SUCCESS;

       }
    }
    /* Check other command substrings similarly*/
    else if (strstr(command, "add_line_after")) {

        if (!add_line_after_command(doc, command)) {

	        valid = SUCCESS;

       }
    }
    else if (strstr(command, "print_document")) {

        if (!print_document_command(doc, command)) {

	        valid = SUCCESS;

        }
    }
    else if (strstr(command, "append_line")) {

        if (!append_line_command(doc, command)) {

	        valid = SUCCESS;

        }
    }   
    else if (strstr(command, "remove_line")) {

       if (!remove_line_command(doc, command)) {

            valid = SUCCESS;

        }
    }
    else if (strstr(command, "load_file")) {

        if (!load_file_command(doc, command)) {

	        valid = SUCCESS;

        }
    }
    else if (strstr(command, "replace_text")) {

        if (!replace_text_command(doc, command)) {

	        valid = SUCCESS;

        }
    }
    else if (strstr(command, "highlight_text")) {

        if (!highlight_text_command(doc, command)) {

	        valid = SUCCESS;

        }
    }
    else if (strstr(command, "remove_text")) {

        if (!remove_text_command(doc, command)) {

	        valid = SUCCESS;

        }
    }
    else if (strstr(command, "save_document")) {

        if (!save_document_command(doc, command)) {

	        valid = SUCCESS;

        }
    }
    else if (strstr(command, "reset_document")) {

        if (!reset_document_command(doc, command)) {

	        valid = SUCCESS;

        }
    }

    /* If the command is invalid, print an error message */
    if (valid) {

        printf("Invalid Command\n");

    }
    
    /* Return the validation status of the command */
    return valid;

}

/***********************************************/
/* This static helper function finds the first */
/* non-space character in a given string.      */
/***********************************************/
static char find_first_non_space(char* str) {

    /* Iterate through the string until the null terminator is reached */
    while (*str != '\0') {

        /* Check if the current character is not a space */
        if (*str != ' ') {

            /* If a non-space character is found, return it */
            return *str;

        }

        /* Move to the next character in the string */
        str++;
    }

    /* Return '\0' if no non-space character is found in the string */
    return '\0';
}

/**********************************************************************/
/* The main function of the program, which serves as the entry point. */
/**********************************************************************/
int main(int argc, char *argv[]) {
    Document doc;
    const char *doc_name = "main_document";
    FILE *input_file;
    char line[MAX_STR_SIZE + 1], line1, line2[MAX_STR_SIZE + 1];
    int num_values;
    size_t byte;

    /* Initialize the document */
    if (init_document(&doc, doc_name)) {

        /* Print error if Initialization fails */
        printf("Initialization failed\n");

    } else {

        /* Check the number of command-line arguments */
        if (argc == 1) {

            /* If there's only one argument, read commands from standard input (stdin) */
            input_file = stdin;

            /* Prompt for user input */
            printf("> ");

            /* Read commands until the user enters "quit" or "exit" */
            while (fgets(line, MAX_STR_SIZE + 1, stdin)) {
                for(byte = 0; line[byte] != '\0'; byte++) {
                    if (line[byte] == '\n') {
                        line[byte] = '\0';
                        break;
                    }
                }
                
                /* Check if the user wants to quit */
                if (!strcmp(line, "quit") || !strcmp(line, "exit")) {
                    break;
                }

                /* Check if the line is not a comment and is not empty */
                if(find_first_non_space(line) != '#' && find_first_non_space(line) != '\0') {

                    /* Parse the command and execute it if it's valid*/
                    num_values = sscanf(line, " %c%s", &line1, line2);

                    /* Check if the parsing was successful */
                    if (num_values > 0) {

                        /* Validate the command */
                        is_valid_command(&doc, line);

                    }
                }

                /* Prompt for the next command */
                printf("> ");
            }
        } else if (argc == 2) {

            /* If there are two arguments, open the specified file for reading */
            input_file = fopen(argv[1], "r");

            /* Check if the file was opened successfully */
            if (input_file == NULL) {

                /* If not, print an error message and exit with an error code */
                fprintf(stderr, "%s cannot be opened.\n", argv[1]);
                exit(EX_OSERR);

            } else {

                /* Read commands from the file until the end of the file or the user enters "quit" or "exit" */
                while (fgets(line, MAX_STR_SIZE + 1, input_file)) {

                    /* Remove the newline character from the input */
                    for (byte = 0; line[byte] != '\0'; byte++) {
                        if (line[byte] == '\n') {
                            line[byte] = '\0';
                            break;
                        }
                    }

                    /* Check if the user wants to quit */
                    if (!strcmp(line, "quit") || !strcmp(line, "exit")) {
                        break;
                    }

                    /* Check if the line is not a comment and is not empty */
                    if(find_first_non_space(line) != '#' && find_first_non_space(line) != '\0') {

                        /* Parse the command and execute it if it's valid */
                        num_values = sscanf(line, " %c%s", &line1, line2);

                        /* Check if the parsing was successful */
                        if (num_values > 0) {

                            /* Validate the Command */
                            is_valid_command(&doc, line);

                        }
                    }
                }
            }
        } else {
            
            /* If there are more than two arguments, print a usage message and exit with an error code */
            fprintf(stderr, "Usage: user_interface\n");
            fprintf(stderr, "Usage: user_interface <filename>\n");
            exit(EX_USAGE);

        }
    }

    /* Close the input file */
    fclose(input_file);

    /* Exit the program with a success status */
    exit(EXIT_SUCCESS);
}