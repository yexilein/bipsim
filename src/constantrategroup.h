

/**
 * @file constantrategroup.h
 * @brief Header for the ConstantRateGroup class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// Multiple include protection
//
#ifndef CONSTANT_RATE_GROUP_H
#define CONSTANT_RATE_GROUP_H

// ==================
//  General Includes
// ==================
//


// ==================
//  Project Includes
// ==================
//
#include "forwarddeclarations.h"
#include "reactiongroup.h"

/**
 * @brief ConstantRateGroup enables simulation of a group of reactions during a time step
 *  for which the reaction rates are supposed to be constant.
 *
 * Under constant rate hypothesis, it is very efficient to compute a chain of reactions.
 * Once the reactions and time step are given, ConstantRateGroup computes the reaction schedule
 * for the time step at once. Access to their timing and performing them is therefore extremely
 * quick.
 * This class inherits class ReactionGroup.
 * @sa ReactionGroup
 * @sa UpdatedRateGroup
 */
class ConstantRateGroup : public ReactionGroup
{
 public:

  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /**
   * @brief Constructor
   * @param reactions A vector of reactions simulated under constant rate hypothesis.
   * @param initial_time Time at the beginning of simulation.
   * @param time_step Time interval during which reactions need to be performed.
   */
  ConstantRateGroup (const std::vector<Reaction*>& reactions, double initial_time, double time_step);

  // Not needed for this class (use of default copy constructor) !
  // /*
  //  * @brief Copy constructor
  //  */
  // ConstantRateGroup ( const ConstantRateGroup& other_constant_rate_group );

  /**
   * @brief Destructor
   */
  virtual ~ConstantRateGroup (void);

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  /**
   * @brief Perform next scheduled reaction and schedule following reaction.
   */
  virtual void perform_next_reaction (void);


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
  // /*
  //  * @brief Assignment operator
  //  */
  // ConstantRateGroup& operator= ( const ConstantRateGroup& other_constant_rate_group );

  // ==================================
  //  Public Methods - Class invariant
  // ==================================
  //
  /**
   * @return True if class invariant is preserved
   */
  virtual bool check_invariant (void) const;


private:

  // ============
  //  Attributes
  // ============
  //
  /** @brief Time points at which reactions occur as determined during construction. */
  std::vector<double> _reaction_times;

  /** @brief Indices of the reaction rates that occur as determined during construction. */
  std::vector<int> _reaction_rate_indices;

  /** @brief Index pointing to the next reaction time and rate index to read. */
  int _next_index;

  // =================
  //  Private Methods
  // =================
  //

  // ======================
  //  Forbidden Operations
  // ======================
  //

};

// ======================
//  Inline declarations
// ======================
//

#endif // CONSTANT_RATE_GROUP_H
