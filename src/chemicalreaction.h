


/**
 * @file chemicalreaction.h
 * @authors Marc Dinh, Stephan Fischer
 * @brief Header for the ChemicalReaction class.
 */


// Multiple include protection
//
#ifndef CHEMICALREACTION_H
#define CHEMICALREACTION_H


// ==================
//  General Includes
// ==================
//
#include <vector> // std::vector

// ==================
//  Project Includes
// ==================
//
#include "forwarddeclarations.h"
#include "bidirectionalreaction.h"

/**
 * @brief This class represents classical chemical reactions.
 *
 * A classical chemical reaction is defined by reactants, producs and their
 * stoichiometry. It usually involves covalent modifications and formation
 * of new elements, which distinguishes it from other reactions.
 */
class ChemicalReaction : public BidirectionalReaction
{
public:
  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /**
   * @brief Constructor
   * @param components Vector of chemicals involved in the reaction.
   * @param stoichiometry Vector of stoichiometry of the chemicals, 
   *  positive for products, negative for reactants.
   * @param forward_rate_constant Forward rate constant.
   * @param backward_rate_constant Backward rate constant.
   * @param forward_bound BoundChemical reactant (if applicable).
   * @param backward_bound BoundChemical product (if applicable).
   */
  ChemicalReaction (const std::vector<FreeChemical*>& components,
		    const std::vector<int>& stoichiometry,
		    double forward_rate_constant,
		    double backward_rate_constant,
		    BoundChemical* forward_bound = 0,
		    BoundChemical* backward_bound = 0);
  
  // Not needed for this class (use of compiler-generated versions)
  // (3-0 rule: either define all 3 following or none of them)
  // /* @brief Copy constructor */
  // ChemicalReaction (ChemicalReaction& other_reaction);
  // /* @brief Assignment operator */
  // ChemicalReaction& operator= (ChemicalReaction& other_reaction);
  // /* @brief Destructor */
  // virtual ~ChemicalReaction (void);

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  // Redefinitions from BidirectionalReaction
  void perform_forward (void);
  void perform_backward (void);

  // ============================
  //  Public Methods - Accessors
  // ============================
  //
  // Redefinitions from BidirectionalReaction
  bool is_forward_reaction_possible (void) const;
  bool is_backward_reaction_possible (void) const;

 private:
  // =================
  //  Private Methods
  // =================
  //
  // redefinitions from BidirectionalReaction
  double compute_forward_rate (void) const;
  double compute_backward_rate (void) const;
  void print (std::ostream& output) const;

  // ============
  //  Attributes
  // ============
  //
  struct CRParticipant 
  { 
    FreeChemical* chemical; 
    int stoichiometry;
  };

  /** @brief Stoichiometry of the forward reactants. */
  std::vector <CRParticipant> _free_reactants;

  /** @brief Stoichiometry of the backward reactants. */
  std::vector <CRParticipant> _free_products;  
  
  /** @brief Bound reactant of the reaction (0 if none). */
  BoundChemical* _bound_reactant;

  /** @brief Bound product of the reaction (0 if none). */
  BoundChemical* _bound_product;
  
  /** @brief Forward rate constant k_1. */
  double _k_1;

  /** @brief Forward rate constant k_-1. */
  double _k_m1;
};

// ======================
//  Inline declarations
// ======================
//

#endif // CHEMICALREACTION_H
