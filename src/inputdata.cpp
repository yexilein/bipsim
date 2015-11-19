

/**
 * @file inputdata.cpp
 * @brief Implementation of the InputData class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// ==================
//  General Includes
// ==================
//
#include <iostream> // std::cout
#include <sstream> // std::istringstream

// ==================
//  Project Includes
// ==================
//
#include "inputdata.h"

// ==========================
//  Constructors/Destructors
// ==========================
//
InputData::InputData (const std::list <std::string>& input_file_names)
  : _file (0)
  , _line_number (0)
  , _file_line_number (0)
  , _line ("")
  , _eof (false)
{
  // open files
  for (std::list <std::string>::const_iterator file_it = input_file_names.begin();
       file_it != input_file_names.end(); file_it++)
    {
      _files.push_back (new std::ifstream (file_it->c_str()));
      if (_files.back()->fail())
	{
	  // TODO: throw exception ?
	  std::cerr << "Warning: could not open input file\n" << *file_it << "\nFile ignored." << std::endl;
	  delete _files.back();
	  _files.pop_back();
	}
      else
	{
	  _file_names.push_back (*file_it);
	}
    }
  _file = _files.begin();
  _file_name = _file_names.begin();

  // go to first line
  go_next();
}

// Not needed for this class (use of default copy constructor) !
// InputData::InputData (const InputData& other_input_data);

InputData::~InputData (void)
{
  for (_file = _files.begin(); _file != _files.end(); ++_file)
    {
      delete *_file;
    }
}

// ===========================
//  Public Methods - Commands
// ===========================
//
void InputData::go_next (void)
{ 
  // look for the next untreated line
  bool found = false;
  while (found == false)
    {
      go_to_next_line();
      if (_eof == true) return; // no more line to parse

      if (is_line_treated() == false)
	{
	  if (is_line_empty())
	    {
	      mark_line_as_treated();
	    }
	  else
	    {
	      found = true;
	    }
	}
    }
}

void InputData::rewind (void)
{
  _eof = false;
  for (_file = _files.begin(); _file != _files.end(); ++_file)
    {
      (*_file)->clear(); // clear flags
      (*_file)->seekg(0); // go to start of file
    }

  _file = _files.begin(); 
  _file_name = _file_names.begin();
  _line_number = 0;
  _file_line_number = 0;
  go_next();
}

void InputData::write_warnings (std::ostream& output)
{
  rewind();

  while (_eof == false)
    {
      std::cout << "Could not make sense of line " << _file_line_number << " in file " << *_file_name << "\n"
		<< "\t" << _line << "\n";
      go_next();
    }
}

// ============================
//  Public Methods - Accessors
// ============================
//


// ==========================
//  Public Methods - Setters
// ==========================
//


// =======================================
//  Public Methods - Operator overloading
// =======================================
//
// Not needed for this class (use of default overloading) !
// InputData& InputData::operator= (const InputData& other_input_data);


// =================
//  Private Methods
// =================
//
void InputData::go_to_next_line (void)
{
  ++_line_number;
  ++_file_line_number;
  
  while ((_file != _files.end()) && (!std::getline (*(*_file), _line))) // end of current file reached
    {
      // go to next file
      ++_file;
      ++_file_name;
      _file_line_number = 1;
    }
  
  if (_file == _files.end())
    {
      _eof = true;
    }
  else
    {
      // mark line as untreated if it is parsed for the first time
      if (_line_treated.size() < _line_number) _line_treated.push_back (false);
    }
}

bool InputData::is_line_empty ()
{
  std::istringstream line_stream (_line);
  std::string first_word;
  if (!(line_stream >> first_word) // line is empty
      || (first_word [0] == '#')) // line is commented
    {
      return true;
    }

  return false;
}
