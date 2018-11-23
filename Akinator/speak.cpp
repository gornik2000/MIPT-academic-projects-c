tree *treeCtor (void);

void speak (char* str)
{
  assert (str != NULL);

  char* speakStr = (char*)calloc (MAX_FILE_STR_LINE, sizeof (*speakStr));

  strcat (speakStr, "balcon -n \"Zira\" -t \"");
  strcat (speakStr, str);
  strcat (speakStr, "\" -s 5 -v 100");

  system (speakStr);
}
