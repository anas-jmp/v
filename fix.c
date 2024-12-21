void handle_append(char **key, char **value)
{
    t_append app;

    app.existing = env_list;
    while (app.existing)
    {
        if (strcmp(app.existing->key, *key) == 0)
        {
            // Get exact lengths
            app.old_len = strlen(app.existing->value);
            app.append_len = strlen(*value);
            // Add 1 for null terminator
            app.new_value = malloc(app.old_len + app.append_len + 1);
            if (!app.new_value)
            {
                free(*value);
                *value = NULL;
                return;
            }
            // Use safer string operations
            app.new_value[0] = '\0';  // Ensure empty string to start
            strlcpy(app.new_value, app.existing->value, app.old_len + 1);
            strlcat(app.new_value, *value, app.old_len + app.append_len + 1);
            free(*value);
            *value = app.new_value;
            break;
        }
        app.existing = app.existing->next;
    }
}
