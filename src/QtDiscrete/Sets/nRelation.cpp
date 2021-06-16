#include "qtdiscrete.h"

N::Relation:: Relation (SUID relate,int lt)
            : relation (relate            )
            , linkType (                lt)
            , flags    (0                 )
            , name     (""                )
            , data     (NULL              )
{
}

N::Relation:: Relation (void)
            : relation (0   )
            , linkType (0   )
            , flags    (0   )
            , name     (""  )
            , data     (NULL)
{
}

N::Relation::~Relation(void)
{
}

N::Relation & N::Relation::operator = (Relation & relation)
{
  return equalTo ( relation ) ;
}

N::Relation & N::Relation::equalTo(Relation & relate)
{
  relation  = relate.relation  ;
  linkType  = relate.linkType  ;
  flags     = relate.flags     ;
  name      = relate.name      ;
  data      = relate.data      ;
  arguments = relate.arguments ;
  nodes     = relate.nodes     ;
  return ME                    ;
}

void N::Relation::connect(Node & first,Node & second)
{
  disconnect ( )                    ;
  nodes << &first                   ;
  nodes << &second                  ;
  for (int i=0;i<nodes.count();i++) {
    nodes[i]->Marriage(this)        ;
  }                                 ;
}

void N::Relation::disconnect(void)
{
  for (int i=0;i<nodes.count();i++) {
    nodes[i]->Divorce(this)         ;
  }                                 ;
  nodes . clear ( )                 ;
}

int N::Relation::join(Node & node)
{
  nodes << &node           ;
  node.Marriage(this)      ;
  return nodes . count ( ) ;
}

int N::Relation::operator += (Node & node)
{
  return join ( node ) ;
}

int N::Relation::Indexing(Node * node)
{
  return nodes . indexOf ( node ) ;
}

bool N::Relation::isFirst(Node * node)
{
  if (nodes.count()<=0) return false ;
  int index = Indexing(node)         ;
  if (index<0) return false          ;
  return ( index == 0 )              ;
}

bool N::Relation::isEnd(Node * node)
{
  if (nodes.count()<=1) return false ;
  int index = Indexing(node)         ;
  if (index<0) return false          ;
  index++                            ;
  return ( index == nodes.count() )  ;
}

bool N::Relation::isDating(void)
{
  return ( nodes . count () == 1 ) ;
}

bool N::Relation::is(N::Groups::Relations relate)
{
  return nEqual ( linkType , (int)relate ) ;
}

int N::Relation::connexion(void) const
{
  return relation ;
}
