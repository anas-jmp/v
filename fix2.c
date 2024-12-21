char **handle_env(t_env *env_list)
{
    extern char **environ;
    t_env *current = env_list;
    int count = 0;
    
    for (char **env = environ; *env; env++)
        count++;
    for (t_env *cur = env_list; cur; cur = cur->next) 
        count++;
        
    char **my_env = malloc((count + 1) * sizeof(char *));
    if (!my_env)
        return (NULL);
    
    int index = 0;
    for (char **env = environ; *env; env++)
    {
        my_env[index] = strdup(*env);
        if (!my_env[index])
        {
            while (--index >= 0)
                free(my_env[index]);
            free(my_env);
            return (NULL);
        }
        index++;
    }

    while (current)
    {
        if (strcmp(current->key, "?") != 0)
        {
            size_t key_len = strlen(current->key);
            size_t value_len = strlen(current->value);
            // Changed +2 to +4 to accommodate '+=+' and '\0'
            my_env[index] = malloc(key_len + value_len + 4);
            if (!my_env[index])
            {
                while (--index >= 0)
                    free(my_env[index]);
                free(my_env);
                return (NULL);
            }
            strcpy(my_env[index], current->key);
            strcat(my_env[index], "=");     // Changed from "+=+"
            strcat(my_env[index], current->value);
            index++;
        }
        current = current->next;
    }
    my_env[index] = NULL;
    return (my_env);
}
