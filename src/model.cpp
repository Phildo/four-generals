#include "model.h"

#include "defines.h"
#include "logger.h"

#include "compass.h"
#include "week.h"
#include "conids.h"

Model::Model()
{
  connections[0] = '0';
  connections[1] = '0';
  connections[2] = '0';
  connections[3] = '0';
  connections[4] = '0';
  zeroAll();
}

void Model::connectCon(char con)
{
  connectionConnection(con) = con;
}

void Model::disconnectCon(char con)
{
  revokeCard(connectionCardinal(con));
  connectionConnection(con) = '0';
}

void Model::assignConCard(char con, char card)
{
  revokeCard(connectionCardinal(con));
  cardinalGeneral(card).connection = con;
  cardinalGeneral(card).cardinal   = card;
}

void Model::revokeCard(char card)
{
  cardinalGeneral(card).connection = '0';
  cardinalGeneral(card).cardinal   = '0';
}

void Model::assignCardinalTurn(char card, Turn t)
{
  cardinalTurn(card) = t;
}

void Model::commitTurns()
{
  char hp[] = {2,2,2,2};
  char card;
  Turn t;
  //first, defenses
  for(int i = 0; i < 4; i++)
  {
    card = Compass::cardinal(i);
    t = cardinalTurn(card);

    if(t.actions[0].what == 'd')
      hp[i]++;
  }
  //then, attacks
  for(int i = 0; i < 4; i++)
  {
    card = Compass::cardinal(i);
    t = cardinalTurn(card);

    if(t.actions[0].what == 'a')
    {
      hp[i]--;
      hp[Compass::icardinal(t.actions[0].who)]--;
    }
  }
  //then, retaliations
  for(int i = 0; i < 4; i++)
  {
    card = Compass::cardinal(i);
    t = cardinalTurn(card);

    if(t.actions[0].what == 'd' && hp[i] == 2) //hp == 1, means retaliate and die. hp == 3 means noone attacked.
    {
      for(int j = 0; j < 4; j++)
      {
        char card2 = Compass::cardinal(j);
        Turn t2 = cardinalTurn(card2);
        if(t2.actions[0].what == 'a' && t2.actions[0].who == card)
        {
          hp[i]--;
          hp[j]--;
        }
      }
    }
  }

  bool loser = false;
  //find losers
  for(int i = 0; i < 4; i++)
  {
    if(hp[i] <= 0)
    {
      victory_status[i]       = 'l';
      victory_status[(i+2)%4] = 'l';
      loser = true;
    }
  }
  //see if theres a tie
  for(int i = 0; loser && i < 4; i++)
  {
    if(victory_status[i] == 'l' && (victory_status[(i+1)%4] == 'l' || victory_status[(i+2)%4] == 'l'))
    {
      for(int j = 0; j < 4; j++)
        victory_status[j] = 't';
    }
  }
  //set winners
  for(int i = 0; loser && i < 4; i++)
  {
    if(victory_status[i] == '0') victory_status[i] = 'w';
  }
  //ascribe victory points
  for(int i = 0; loser && i < 4; i++)
  {
    char card = Compass::cardinal(i);
    if(victory_status[i] == 'w') connectionVictoryRecord(cardinalConnection(card)).win++;
    if(victory_status[i] == 'l') connectionVictoryRecord(cardinalConnection(card)).loss++;
    if(victory_status[i] == 't') connectionVictoryRecord(cardinalConnection(card)).tie++;
  }

  zeroTomorrowsTurns();
  days++;
}


//routers

char& Model::connectionConnection(char con)
{
  return connections[ConIds::iconid(con)];
}
VictoryRecord& Model::connectionVictoryRecord(char con)
{
  return connectionRecords[ConIds::iconid(con)];
}
char Model::connectionCardinal(char con)
{
  for(int i = 0; i < 4; i++)
    if(generals[i].connection == con) return generals[i].cardinal;
  return '0';
}


char& Model::cardinalConnection(char card)
{
  return connectionConnection(cardinalGeneral(card).connection);
}
General& Model::cardinalGeneral(char card)
{
  return generals[Compass::icardinal(card)];
}
Turn& Model::cardinalTurn(char card)
{
  return cardinalDayTurn(card, days);
}
Turn& Model::cardinalDayTurn(char card, int day)
{
  if(day < 0 || day >= FG_MAX_ACTION_HIST || day > days) return turns[FG_MAX_ACTION_HIST*4];
  return turns[(day*4)+Compass::icardinal(card)];
}

bool Model::cardinalMessage(char card, Action& a)
{
  a.zero();
  if(days < 2) return false;

  char p = Compass::opcardinal(card);

  //if partner sent message
  Turn t = cardinalDayTurn(p, days-2);
       if(t.actions[0].what == 'm') a = t.actions[0];
  else if(t.actions[1].what == 'm') a = t.actions[1];
  else                              return false;

  //if route was sabotaged
  t = cardinalDayTurn(a.route, days-2);
       if(t.actions[0].what == 's') a.beSabotaged(t.actions[0]);
  else if(t.actions[1].what == 's') a.beSabotaged(t.actions[1]);

  //if sabotage was block
  if((t.actions[0].what == 's' && t.actions[0].how == 'b') &&
     (t.actions[0].what == 's' && t.actions[0].how == 'b'))
  {
    a.zero();
    return false;
  }

  return true;
}

bool Model::cardinalIntruder(char card, Action& a0, Action& a1)
{
  a0.zero();
  a1.zero();
  if(days < 1) return false;

  char e;
  Turn t;

  e = Compass::cwcardinal(card);
  t = cardinalDayTurn(e, days-1);
  if(t.actions[0].what == 'm' && t.actions[0].route == card)
      a0 = t.actions[0];
  if(t.actions[1].what == 'm' && t.actions[1].route == card)
      a0 = t.actions[1];

  e = Compass::ccwcardinal(card);
  t = cardinalDayTurn(e, days-1);
  if(t.actions[0].what == 'm' && t.actions[0].route == card)
  {
    if(a0.what == '0') a0 = t.actions[0];
    else               a1 = t.actions[0];
  }
  if(t.actions[1].what == 'm' && t.actions[1].route == card)
  {
    if(a0.what == '0') a0 = t.actions[1];
    else               a1 = t.actions[1];
  }

  return (a0.what != '0');
}

bool Model::cardinalSabotage(char card, Action& a0, Action& a1)
{
  a0.zero();
  a1.zero();

  Turn t = cardinalDayTurn(card, days-1);
  Action a;
       if(t.actions[0].what == 's') a = t.actions[0];
  else if(t.actions[1].what == 's') a = t.actions[1];
  else                              return false;

  if(a.how != 'r') return false;
  else return cardinalIntruder(card, a0, a1);
}

bool Model::cardinalConnected(char card)
{
  return cardinalGeneral(card).connection != '0';
}

bool Model::connectionConnected(char con)
{
  return connectionConnection(con) != '0';
}

bool Model::cardinalHasTurn(char card)
{
  return cardinalTurn(card).cardinal != '0';
}

bool Model::cardinalWin(char card)
{
  return victory_status[Compass::icardinal(card)] == 'w';
}

bool Model::cardinalLose(char card)
{
  return victory_status[Compass::icardinal(card)] == 'l';
}

bool Model::cardinalTie(char card)
{
  return victory_status[Compass::icardinal(card)] == 't';
}

bool Model::roundOver()
{
  return victory_status[0] != '0';
}

bool Model::rolesAssigned()
{
  return
    cardinalConnected('n') &&
    cardinalConnected('e') &&
    cardinalConnected('s') &&
    cardinalConnected('w');
}

bool Model::turnsAssigned()
{
  return
    cardinalHasTurn('n') &&
    cardinalHasTurn('e') &&
    cardinalHasTurn('s') &&
    cardinalHasTurn('w');
}

char Model::currentDay()
{
  return Week::day((days+7)%7); //+7 for obvious cross-lang modulus behavior
}

void Model::zeroCurrentTurns()
{
  if(days < 0 || days >= FG_MAX_ACTION_HIST) return;
  for(int i = 0; i < 5; i++)
    turns[(days*4)+i].zero();
}

void Model::zeroTomorrowsTurns()
{
  if(days < -1 || days >= (FG_MAX_ACTION_HIST-1)) return;
  for(int i = 0; i < 5; i++)
    turns[((days+1)*4)+i].zero();
}

void Model::zeroRound()
{
  days = -1; //set to "not yet a valid day"
  zeroTomorrowsTurns();
  for(int i = 0; i < 4; i++)
    victory_status[i] = '0';
}

void Model::zeroAll()
{
  zeroRound();
  //don't actually know how to handle this yet...
  //probably zero victory records?
}


Model::~Model()
{
}

