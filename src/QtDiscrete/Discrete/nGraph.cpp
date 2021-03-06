#include "qtdiscrete.h"

N::Graph:: Graph    (void )
         : destruct (false)
{
}

N::Graph::~Graph (void)
{
  if (destruct) Purify ( ) ;
}

void N::Graph::Purify(void)
{
  if (nodes.count()>0)                  {
    for (int i=0;i<nodes.count();i++)   {
      delete nodes[i]                   ;
    }                                   ;
  }                                     ;
  if (linkers.count()>0)                {
    for (int i=0;i<linkers.count();i++) {
      delete linkers[i]                 ;
    }                                   ;
  }                                     ;
  nodes   . clear ( )                   ;
  linkers . clear ( )                   ;
}

bool N::Graph::addRelation(Relation * relate)
{
  if (linkers.contains(relate)) return false ;
  linkers << relate                          ;
  return true                                ;
}

bool N::Graph::removeRelation(Relation * relate)
{
  if (!linkers.contains(relate)) return false ;
  int index = linkers.indexOf(relate)         ;
  if (index<0) return false                   ;
  linkers.takeAt(index)                       ;
  return true                                 ;
}

bool N::Graph::addNode(Node * node)
{
  if (nodes.contains(node)) return false ;
  nodes << node                          ;
  return true                            ;
}

bool N::Graph::removeNode(Node * node)
{
  if (!nodes.contains(node)) return false ;
  int index = nodes.indexOf(node)         ;
  if (index<0) return false               ;
  detachNode(node)                        ;
  nodes.takeAt(index)                     ;
  return true                             ;
}

void N::Graph::detachNode(Node * node)
{
  Q_UNUSED ( node ) ;
}

void N::Graph::Mount(Relation * relate,Node * first,Node * second)
{
  addRelation       ( relate           ) ;
  addNode           ( first            ) ;
  addNode           ( second           ) ;
  relate -> connect ( *first , *second ) ;
}

int N::Graph::Members(void)
{
  return nodes.count() ;
}

N::Node * N::Graph::Member(int index)
{
  if (nodes.count()<=index) return NULL ;
  return nodes[index]                   ;
}

bool N::Graph::contains(Node * node)
{
  return nodes.contains(node) ;
}
