
/**
 * @file manualdispatchsolver.cpp
 * @brief Implementation of the ManualDispatchSolver class.
 * @authors Marc Dinh, Stephan Fischer
 */

// ==================
//  General Includes
// ==================
//
#include <iostream>

// ==================
//  Project Includes
// ==================
//
#include "manualdispatchsolver.h"
#include "reactionclassification.h"
#include "updatedrategroup.h"
#include "constantrategroup.h"

// ==========================
//  Constructors/Destructors
// ==========================
//
ManualDispatchSolver::
ManualDispatchSolver (const SimulationParams& params, 
		      const ReactionClassification& classification)
  : Solver (params)
  , _next_reaction (0)
  , _next_reaction_time (NO_REACTION_LEFT)
{
  int number_groups = classification.number_classes();

  // create a group for every class in the reaction
  for (int i = 0; i < number_groups; ++i)
    {
      if (classification.time_step (i) 
	  == ReactionClassification::ALWAYS_UPDATED)
	{
	  // if rates are always updated, we use UpdatedRateGroup
	  _updated_rate_groups.push_back 
	    (new UpdatedRateGroup (params, classification.reactions (i),
				   time()));
	}
      else
	{
	  // else we use ConstantRateGroup with the provided time step
	  ConstantRateGroup* group = 
	    new ConstantRateGroup (params, classification.reactions (i), 
				   time(), classification.time_step(i));
	  _constant_rate_groups.push_back (group);
	  // schedule event
	  insert_event (group->next_reaction_time(), group);
	}
    }
  schedule_next_reaction();
}

// Forbidden
// ManualDispatchSolver::ManualDispatchSolver (const ManualDispatchSolver& other_solver);
// ManualDispatchSolver& ManualDispatchSolver::operator= (const ManualDispatchSolver& other_solver);

ManualDispatchSolver::~ManualDispatchSolver (void)
{  
  // delete all allocated reaction groups
  for (int i = 0; i < _constant_rate_groups.size(); ++i)
    { delete _constant_rate_groups [i]; }
  for (int i = 0; i < _updated_rate_groups.size(); ++i)
    { delete _updated_rate_groups [i]; }
}

// ===========================
//  Public Methods - Commands
// ===========================
//
void ManualDispatchSolver::schedule_next_reaction (void)
{
  // pop previous reaction and replace it if necessary
  if (_event_list.front().group != _variable_event.group) 
    { update_first_event(); }

  // update events generated by variable rate groups
  update_variable_event();

  // depending on reactant availability, we may need to go through several 
  // reactions before we find a reaction that can actually be performed
  // correctly
  bool reaction_found = false;
  while (reaction_found == false)
    {
      // _next_reaction can be 0 if a group reached the limit of its time step
      // scheduling a new reaction in the group will trigger its update
      _next_reaction = _event_list.front().group->next_reaction();
      if ((_next_reaction != 0) && (_next_reaction->is_reaction_possible()))
	{
	  _next_reaction_time = _event_list.front().time;
	  reaction_found = true;
	}
      else { update_first_event(); }
    }
}

// ============================
//  Public Methods - Accessors
// ============================
//

// =================
//  Private Methods
// =================
//
void ManualDispatchSolver::reinitialize (void)
{
  // we reinitialize and reschedule all constant rate events
  _event_list.clear();
  for (std::vector <ConstantRateGroup*>::iterator 
	 it = _constant_rate_groups.begin();
       it != _constant_rate_groups.end(); ++it)
    {
      (*it)->reinitialize (time());
      insert_event ((*it)->next_reaction_time(), *it);
    }

  // we reinitialize updated rate groups
  for (std::vector <UpdatedRateGroup*>::iterator 
	 it = _updated_rate_groups.begin();
       it != _updated_rate_groups.end(); ++it)
    {
      (*it)->reinitialize (time());
    }
}

void ManualDispatchSolver::update_first_event (void)
{
  ReactionGroup* group = _event_list.front().group;
  _event_list.pop_front();
  group->schedule_next_reaction (time());
  insert_event (group->next_reaction_time(), group);
}

void ManualDispatchSolver::insert_event (double time, ReactionGroup* group)
{
  // insert event at its right position in the list (sorted by event time)
  std::list <GroupEvent>::iterator it = _event_list.begin();
  while ((it != _event_list.end()) && (it->time < time)) { ++it; }
  _event_list.insert (it, GroupEvent (time, group));
}

void ManualDispatchSolver::update_variable_event (void)
{
  // remove previous event if necessary
  std::list <GroupEvent>::iterator it = _event_list.begin();
  while ((it != _event_list.end()) && (it->group != _variable_event.group)) 
    { ++it; }
  if (it != _event_list.end()) { _event_list.erase (it); }
  
  // we update variable rate groups and keep an event for the next reaction
  _variable_event.time = NO_REACTION_LEFT;
  for (std::vector <UpdatedRateGroup*>::iterator 
	 it = _updated_rate_groups.begin();
       it != _updated_rate_groups.end(); ++it)
    {
      (*it)->schedule_next_reaction (time());
      if ((*it)->next_reaction_time() < _variable_event.time)
	{
	  _variable_event.group = *it;
	  _variable_event.time = (*it)->next_reaction_time();
	}
    }
  insert_event (_variable_event.time, _variable_event.group);
}

