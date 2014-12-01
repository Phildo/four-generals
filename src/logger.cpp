#include "logger.h"

MemLog *MemLog::sing;

MemLog::MemLog()
{
  log_i = 0;
  log_c = 0;
}

MemLog * MemLog::inst()
{
  if(!sing) sing = new MemLog();
  return sing;
}

void MemLog::log(char *buff)
{
  char *l = &logs[(log_i%FG_MAX_NUM_LOGS)*FG_MAX_LOG_LEN];
  int i = 0 ;
  while(buff[i] != '\0' && i < FG_MAX_LOG_LEN-1)
  {
    l[i] = buff[i];
    i++;
  }
  l[i] = '\0';
  log_i++;
  log_c++;
  if(log_c > FG_MAX_NUM_LOGS) log_c = FG_MAX_NUM_LOGS;
}

char *MemLog::logAt(int i)
{
  return logs+(((log_i-log_c+FG_MAX_NUM_LOGS+i)%FG_MAX_NUM_LOGS)*FG_MAX_LOG_LEN);
}

