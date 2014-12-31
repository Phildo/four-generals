#include "victory_record.h"

VictoryRecord::VictoryRecord() : win(0), loss(0), tie(0)
{
}

VictoryRecord::~VictoryRecord()
{

}

void VictoryRecord::reset()
{
  win = 0;
  loss = 0;
  tie = 0;
}

