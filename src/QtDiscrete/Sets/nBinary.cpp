#include "qtdiscrete.h"

N::Binary:: Binary (void)
{
  nodes << new Node ( ) ;
  nodes << new Node ( ) ;
}

N::Binary::~Binary (void)
{
  Purify ( ) ;
}

void N::Binary::Purify(void)
{
  N::clean ( nodes ) ;
}

void N::Binary::Relate (SUID first,SUID second,int relate)
{
  relation     . linkType = relate                 ;
  nodes [ 0 ] -> node     = first                  ;
  nodes [ 1 ] -> node     = second                 ;
  relation . connect ( *(nodes[0]) , *(nodes[1]) ) ;
}

bool N::Binary::Pair(QString valuePair)
{
  using namespace N::Groups        ;
  //////////////////////////////////
  QStringList L                    ;
  relation.linkType = 0            ;
  if (valuePair.contains('='))     {
    L = valuePair.split('=')       ;
    if (L.count()==2)              {
      nodes[0]->name = L[0]        ;
      nodes[1]->name = L[1]        ;
      Relate(1,2,Equivalent)       ;
      return true                  ;
    }                              ;
  } else
  if (valuePair.contains('@'))     {
    relation.linkType = Contains   ;
    L = valuePair.split('@')       ;
    if (L.count()==2)              {
      nodes[0]->name = L[0]        ;
      nodes[1]->name = L[1]        ;
      Relate(1,2,Contains)         ;
      return true                  ;
    }                              ;
  }  else
  if (valuePair.contains('>'))     {
    relation.linkType = Greater    ;
    L = valuePair.split('>')       ;
    if (L.count()==2)              {
      nodes[0]->name = L[0]        ;
      nodes[1]->name = L[1]        ;
      Relate(1,2,Greater)          ;
      return true                  ;
    }                              ;
  }  else
  if (valuePair.contains('<'))     {
    relation.linkType = Less       ;
    L = valuePair.split('<')       ;
    if (L.count()==2)              {
      nodes[0]->name = L[0]        ;
      nodes[1]->name = L[1]        ;
      Relate(1,2,Less)             ;
      return true                  ;
    }                              ;
  }                                ;
  return false                     ;
}

bool N::Binary    :: SqlWhere (
       QString     & Where    ,
       QStringList & Keys     ,
       KMAPs       & Maps     ,
       QString       Options  )
{
  using namespace N::Groups                            ;
  nKickOut ( nodes . count ( ) < 2 , false )           ;
  //////////////////////////////////////////////////////
  QString     A = nodes[0]->name                       ;
  QString     B = nodes[1]->name                       ;
  QString     a = A.toLower()                          ;
  QString     U = a.toUpper()                          ;
  QString     J = ""                                   ;
  int         R = relation.linkType                    ;
  QStringList Pairs                                    ;
  //////////////////////////////////////////////////////
  if (R==Equivalent) J = "="                      ; else
  if (R==Contains  )                                   {
    J = "like"                                         ;
    if (!B.contains('%'))                              {
      B . prepend ( '%' )                              ;
      B . append  ( '%' )                              ;
    }                                                  ;
  } else return false                                  ;
  //////////////////////////////////////////////////////
  nKickOut ( J . length ( ) <= 0 , false )             ;
  //////////////////////////////////////////////////////
  Maps [ a ] = B                                       ;
  Keys  << a                                           ;
  Pairs << QString("%1 %2 %3").arg(a).arg(J).arg(U)    ;
  if (Options.length()>0) Pairs << Options             ;
  Where = QString("where %1").arg(Pairs.join(" and ")) ;
  return true                                          ;
}
