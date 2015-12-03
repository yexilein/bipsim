

/**
 * @file ratenode.h
 * @brief Header for the ClassName class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// Multiple include protection
//
#ifndef RATE_NODE_H
#define RATE_NODE_H

// ==================
//  General Includes
// ==================
//


// ======================
//  Forward declarations
// ======================
//
#include "forwarddeclarations.h"

/**
 * @brief Abstract class used to store rates in a tree.
 *
 * RateNode stores rate in a tree structure that can be updated when prompted
 * by user. Inheriting classes should be able to store a rate. Apart from 
 * the classical node structure, they can be invalidated (because there rate
 * is not to date) and updated.
 * @sa RateTree
 */
class RateNode
{
 public:

  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /**
   * @brief Default constructor.
   */
  RateNode (void)
    : _parent (0)
    , _invalidated (false)
    , _rate (0)
    {
    }

  // Not needed for this class (use of default copy constructor) !
  // /*
  //  * @brief Copy constructor.
  //  */
  // RateNode (const RateNode& other_class_name);

  /**
   * @brief Destructor.
   */
  ~RateNode (void) {}

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  /**
   * @brief Update rate if necessary.
   * @return Parent node that will need an update due to update propagating
   *  to higher levels of the tree. Return 0 if parent does not need update
   *  or has already been invalidated (and thus should already be listed for
   *  update somewhere else).
   */
  RateNode* update (void)
  {
    perform_update();
    _invalidated = false;
    return invalidate_parent();
  }

  /**
   * @brief Find value in the tree.
   * @param value Cumulated rate value.
   * @return Integer corresponding to a rate index. Each node will typically
   *  propagate the request to the child encompassing the requested value until
   *  a leaf is reached which will return the rate index it holds.
   */
  virtual int find (double value) = 0;

  /**
   * @brief Mark node as needing an update.
   */
  void invalidate (void) { _invalidated = true; }

  // ============================
  //  Public Methods - Accessors
  // ============================
  //
  /**
   * @brief Accessor to rate stored by the node.
   * @return Rate as of last update.
   */
  double rate (void) { return _rate; }

  /**
   * @brief Accessor to parent of the node.
   * @return Parent.
   */
  RateNode* parent (void) { return _parent; }

  /**
   * @brief Accessor to invalidation status.
   * @return True if node has been invalidated since last update.
   */
  bool is_invalidated (void) { return _invalidated; }

  // ==========================
  //  Public Methods - Setters
  // ==========================
  //
  /**
   * @brief Setter for parent.
   * @param parent Pointer to new parent for the node.
   */
  void set_parent (RateNode* parent) { _parent = parent; }

  // =======================================
  //  Public Methods - Operator overloading
  // =======================================
  //
  // Not needed for this class (use of default overloading) !
  // /*
  //  * @brief Assignment operator.
  //  */
  // RateNode& operator= (const RateNode& other_class_name);

protected:
  // ======================
  //  Protected Attributes
  // ======================
  //

  // ===================
  //  Protected Methods
  // ===================
  //
  /**
   * @brief Setter for rate.
   * @param value New rate held by the node.
   */
  void _set_rate (double value) { _rate = value; }

  /**
   * @brief Invalidate parent if needed.
   * @return Pointer to parent if it has not already been invalidate, 0 if it
   *  already has.
   */
  RateNode* invalidate_parent (void)
  {
    if ((_parent == 0) || _parent->is_invalidated()) return 0;
    else { _parent->invalidate(); return _parent; }
  }

private:
  // ============
  //  Attributes
  // ============
  //
  /** @brief Parent of the node. */
  RateNode* _parent;

  /** @brief Rate stored in the node. */
  double _rate;

  /** @brief Flag indicating if the node needs to be updated. */
  bool _invalidated;

  // =================
  //  Private Methods
  // =================
  //
  /**
   * @brief Update rate (without worrying about propagation to parent).
   */
  virtual void perform_update (void) = 0;
  
  // ======================
  //  Forbidden Operations
  // ======================
  //

};

// ======================
//  Inline declarations
// ======================
//

#endif // RATE_NODE_H
