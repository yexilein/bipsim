

/**
 * @file simulation.cpp
 * @brief Implementation of the Simulation class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// ==================
//  General Includes
// ==================
//
#include <string> // std::string
#include <iostream>

// ==================
//  Project Includes
// ==================
//
#include "simulation.h"
#include "cellstate.h"
#include "chemical.h"
#include "solverfactory.h"
#include "chemicallogger.h"
#include "doublestrandlogger.h"
#include "eventhandler.h"
#include "volumehandler.h"
#include "inputdata.h"
#include "parser.h"
#include "randomhandler.h"
#include "doublestrand.h"


// ==========================
//  Constructors/Destructors
// ==========================
//
Simulation::Simulation (const std::string& filename)
  : _params (filename)
  , _logger (0)
  , _replication_logger (0)
  , _solver (0)
  , _next_log_time (0)
  , _next_timing (0)
  , _next (EVENT)
  , _volume_handler (0)
{
  RandomHandler::instance().set_seed (_params.seed());

  // read input files and create units, reactions and events
  InputData input_data (_params.input_files());
  Parser parser (_cell_state, _event_handler);
  parser.parse (input_data);
  input_data.write_warnings (std::cerr);
  std::cout << "Loaded system containing "
	    << _cell_state.number_chemicals() << " chemicals and "
	    << _cell_state.reactions().size() << " reactions."
	    << std::endl;

  // create volume handler
  _volume_handler = new VolumeHandler (1);
  _cell_state.set_volume (_volume_handler->initial_value());

  // create solver
  _solver = _params.solver_factory().create (_params, _cell_state);

  // create logger
  _next_log_time = _params.initial_time();
  _logger = create_chemical_logger();
  // _replication_logger = new DoubleStrandLogger 
  //   (_params.replication_file(), 
  //    *_cell_state.find <DoubleStrand> (_params.output_double_strand()));
}

// Forbidden
// Simulation::Simulation (const Simulation& other_simulation);
// Simulation& Simulation::operator= (const Simulation& other_simulation);

Simulation::~Simulation (void)
{
  delete _replication_logger;
  delete _logger;
  delete _solver;
  delete _volume_handler;
}

// ===========================
//  Public Methods - Commands
// ===========================
//
void Simulation::run (void)
{
  std::cout << "Solving from t = " << _params.initial_time()
	    << " to t = "<< _params.final_time() << "..." << std::endl;

  // ignore all events preceding simulation start
  while (_event_handler.next_event_time() < _params.initial_time())
    { _event_handler.ignore_event(); }

  compute_next_timing();
  while (_next_timing < _params.final_time())
    {
      write_logs (_next_timing);
      if (_next == REACTION)
	{ 
	  _solver->perform_next_reaction(); 
	}
      else if (_next == EVENT)
	{ 
	  _event_handler.perform_event(); 
	  _solver->reschedule (_event_handler.next_event_time());
	}
      else if (_next == VOLUME)
	{
	  _cell_state.set_volume (_volume_handler->next_value());
	  _solver->reschedule (_volume_handler->next_time());
	  _volume_handler->step();
	}
      else
	{
	  std::cerr << "Impossible case in simulation loop..." << std::endl;
	}
      compute_next_timing();
    }
  write_logs (_params.final_time());

  std::cout << "\n" << _solver->number_reactions_performed() 
	    << " reactions occurred." << std::endl;
}

// ============================
//  Public Methods - Accessors
// ============================
//

// =================
//  Private Methods
// =================
//
ChemicalLogger* Simulation::create_chemical_logger (void)
{
  std::list <std::string> chemical_names = _params.output_entities();
  std::list <const Chemical*> chemical_refs;
  std::list <std::string>::iterator name_it = chemical_names.begin();
  while (name_it != chemical_names.end())
    {
      const Chemical* next_chemical = _cell_state.find <Chemical> (*name_it);
      if (next_chemical != 0)
	{ chemical_refs.push_back (next_chemical); ++name_it; }
      else
	{
	  std::cerr << "WARNING: unknown chemical " << *name_it
		    << " will not be logged!\n";
	  name_it = chemical_names.erase (name_it);
	}
    }
  return new ChemicalLogger (_params.concentration_file(),
			     chemical_refs, chemical_names);
}

void Simulation::compute_next_timing (void)
{
  _next_timing = _event_handler.next_event_time();
  _next = EVENT;
  if (_volume_handler->next_time() < _next_timing)
    {
      _next_timing = _volume_handler->next_time();
      _next = VOLUME;
    }
  if (_solver->next_reaction_time() < _next_timing)
    {
      _next_timing = _solver->next_reaction_time();
      _next = REACTION;
    }
}

void Simulation::write_logs (double t)
{
  while (t >= _next_log_time)
    { 
      _logger->log (_next_log_time); 
      if (_replication_logger) { _replication_logger->log (_next_log_time); }
      _next_log_time += _params.output_step();
    }
}
