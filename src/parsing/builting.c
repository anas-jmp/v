#include "../../include/minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define INITIAL_REDIRECT_SIZE 4 // Initial size for redirection arrays
#define INITIAL_ARGS_SIZE 16


// Function to handle `echo` command
char *process_echo_command(const char **str, token_type *type)
{
    *str += 4;  
    *type = WORD;
    return strdup("echo");
}

 char *process_unset_command(const char **str, token_type *type) 
{

    *str += 5; // Move past 'unset'
 
     while (isspace(**str))
        (*str)++;

    while (**str && !isspace(**str)) 
    {
         const char *variable_start = *str;
        while (**str && !isspace(**str))
            (*str)++;
         size_t len = *str - variable_start;
        if (len > 0) {
            char *variable_name = malloc(len + 1);
            strncpy(variable_name, variable_start, len);
            variable_name[len] = '\0';

             printf("\nUnsetting variable: [%s]\n", variable_name);
            handle_unset(variable_name);
            
            free(variable_name);
        }
        // Skip any additional spaces between variables
        while (isspace(**str))
            (*str)++;
    }

    *type = WORD;
    return strdup("unset");
 
}

char *process_export_command(const char **str, token_type *type) 
{
 
     *str += 6;
     char *remain = strdup(*str);
     if(remain!=NULL)
     {
         handle_export_command(remain);
     }
      *type = WORD;
    return strdup("export");

}

char *next_tok2(const char *start, const char **str, token_type *type)
{
    // if (strncmp(start, "echo", 4) == 0 && (isspace(start[4]) || start[4] == '\0'))
    // {
    //      return process_echo_command(str, type);  
    // }

    if (strncmp(start, "unset", 5) == 0 && (isspace(start[5]) || start[5] == '\0')) 
    {
        return process_unset_command(str, type);  
    }

    if (strncmp(start, "export", 6) == 0 && (isspace(start[6]) || start[6] == '\0'))
    {
        return process_export_command(str, type); // Call the dedicated export handler
    }

    return NULL;  
}

char *next_tok3(const char **str, char **env, token_type *type)
{
 t_token_data token_data;
    
     token_data.quote = **str;  
    (*str)++;            
     token_data.quoted_start = *str;
     while (**str && **str != token_data.quote)
        (*str)++;
      token_data.len = *str - token_data.quoted_start;  
      token_data.token_value = malloc(token_data.len + 1);
    strncpy(token_data.token_value, token_data.quoted_start, token_data.len);
    token_data.token_value[token_data.len] = '\0';  
    if (**str == token_data.quote)
        (*str)++;  
    if (token_data.quote == '\'') 
    {
        *type = SINGLE_QUOTED_STRING;
        return token_data.token_value; // Single quotes: return the content as-is
    } 
    else 
    {
       *type = DOUBLE_QUOTED_STRING;
        token_data.expanded_token = handle_quoted_string_with_expansion(token_data.token_value, env);
        free(token_data.token_value);
        return token_data.expanded_token; // Double quotes: expand variables
    }
    return NULL;  
}