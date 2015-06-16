

/**
 * @file binding.cpp
 * @brief Implementation of the BaseLoading class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// ==================
//  General Includes
// ==================
//
#include <iostream> // std::cout

// ==================
//  Project Includes
// ==================
//
#include "macros.h" // REQUIRE()
#include "boundchemical.h"
#include "baseloading.h"
#include "baseloader.h"

// ==========================
//  Constructors/Destructors
// ==========================
//
BaseLoading::BaseLoading (BaseLoader& base_loader, BoundChemical& occupied_loader)
  : _base_loader (base_loader)
  , _occupied_loader (occupied_loader)
{
}
 
// Not needed for this class (use of default copy constructor) !
// BaseLoading::BaseLoading (BaseLoading& other_base_loading);

BaseLoading::~BaseLoading (void)
{
}

// ===========================
//  Public Methods - Commands
// ===========================
//
void BaseLoading::perform_forward (void)
{
  REQUIRE( forward_rate() > 0 ); /** @pre There are enough chemicals to perform reaction. */

  // Choose one of the base loaders randomly
  _base_loader.focus_random_unit_from_loading_rates();
  
  // Load the element corresponding to its target template
  _base_loader.focused_base_to_load().remove (1);
  // currently the base just disappears into thin air. It will show up eventually
  // when the elongated chemical is released so let us say it does virtually exist :)
  
  // Update the base loader status to occupied
  _occupied_loader.add_unit_in_place_of (_base_loader);
  _base_loader.focused_unit_location().replace_bound_unit (_base_loader, _occupied_loader);      
  _base_loader.remove_focused_unit();
}

void BaseLoading::perform_backward( void )
{
  std::cerr << "ERROR: There is no backward reaction to base loading defined currently." << std::endl;
}


void BaseLoading::print (std::ostream& output) const
{
  output << "BaseLoading reaction.";
}


// ============================
//  Public Methods - Accessors
// ============================
//
double BaseLoading::forward_rate (void) const
{
  /**
   * Loading rate is generally defined by r = sum ( k_on_i x [A] x [B_i], where [A] is the
   * concentration of BaseLoader and [B_i] the concentration of base i. k_on_i may vary from a
   * base to another. All this information is stored within the 
   *   r_total = [A] sum ( k_on_i x [B_i] ) = [A] vector(k_on_i).vector([B_i])
   * The concentration of units to bind is easy to get, the binding site handler can compute
   * the rest of the formula for us as it holds information about binding rates and binding site
   * concentrations.
   */
  return _base_loader.loading_rate ();
}

double BaseLoading::backward_rate (void) const
{
  return 0;
}


// ==========================
//  Public Methods - Setters
// ==========================
//


// =======================================
//  Public Methods - Operator overloading
// =======================================
//
// Not needed for this class (use of default overloading) !
// BaseLoading& BaseLoading::operator= (BaseLoading& other_base_loading);

// ==================================
//  Public Methods - Class invariant
// ==================================
//
/**
 * Checks all the conditions that must remain true troughout the life cycle of
 * every object.
 */
bool BaseLoading::check_invariant (void) const
{
  bool result = Reaction::check_invariant();
  return result;
}


// =================
//  Private Methods
// =================
//
