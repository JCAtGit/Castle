#include "MarkerDefinition.h"
/**
 * Highly Coupled Resource Management System
 *
 * This program demonstrates high coupling in C:
 * - All functions share and modify global resources, logs, and counters.
 * - Functions call each other extensively (useResource -> obtainResource ->
 * convertResourceToGetTarget -> deductResource/addResource).
 * - Most functions depend on global state and each other's behavior.
 * - Changing one function (e.g., conversion efficiency) will affect many
 * others.
 * - Circular dependencies exist: useResource, obtainResource,
 * convertResourceToGetTarget are tightly interwoven.
 * - Global operation counter and logs are updated by almost every function.
 */

#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

/* ---------- Global Constants ---------- */
#define RESOURCE_COUNT 3
#define WATER 0
#define ELECTRICITY 1
#define FUEL 2

#define CONV_EFFICIENCY                                                        \
  0.8 /* Conversion loss: 80% of source becomes target                         \
       */
#define MAX_LOG_ENTRIES 100

/* ---------- Highly Coupled Global State ---------- */
double resources[RESOURCE_COUNT]; /* Current levels of each resource */
int totalOperations = 0; /* Counter incremented by every state change */
int lastError = 0;       /* Global error flag (0 = OK, non-zero = error code) */
char logMessages[MAX_LOG_ENTRIES][256];
int logCount = 0;

/* ---------- Function Prototypes (all interdependent) ---------- */
void logEvent(const char *fmt, ...);
bool deductResource(int resource, double amount);
bool addResource(int resource, double amount);
bool convertResourceToGetTarget(int fromRes, int toRes, double targetAmount,
                                double efficiency);
bool obtainResource(int resource, double neededAmount);
bool useResource(int resource, double amount);
bool produceResource(int resource, double amount);
void printResources(const char *label);
void printLogs(void);

/* ---------- Implementation (High Coupling Demonstrated) ---------- */

/**
 * Logs a formatted message into the global circular log buffer.
 * Called by almost every other function -> high coupling.
 */
void logEvent(const char *fmt, ...) {
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(0); 
  __asm__ volatile("" ::: "memory");
  if (logCount >= MAX_LOG_ENTRIES) {
    /* Log full: shift entries or ignore; for simplicity ignore new logs */
    return;
  }
  va_list args;
  va_start(args, fmt);
  vsnprintf(logMessages[logCount], sizeof(logMessages[0]), fmt, args);
  va_end(args);
  logCount++;
}

/**
 * Raw deduction: subtracts amount from resource, logs, updates global op
 * counter. No attempt to obtain missing resource – used internally to avoid
 * recursion.
 */
bool deductResource(int resource, double amount) {
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(1); 
  __asm__ volatile("" ::: "memory");
  if (resource < 0 || resource >= RESOURCE_COUNT || amount < 0) {
    lastError = 1;
    __asm__ volatile("" ::: "memory");
    codemarker_alpha(2); 
    __asm__ volatile("" ::: "memory");
    logEvent("ERROR: Invalid deductResource call (res=%d, amt=%.2f)", resource,
             amount);
    return false;
  }
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(3); 
  __asm__ volatile("" ::: "memory");
  if (resources[resource] >= amount) {
    resources[resource] -= amount;
    totalOperations++;
    __asm__ volatile("" ::: "memory");
    codemarker_alpha(4); 
    __asm__ volatile("" ::: "memory");
    logEvent("Deducted %.2f from %s (new level=%.2f)", amount,
             resource == WATER
                 ? "WATER"
                 : (resource == ELECTRICITY ? "ELECTRICITY" : "FUEL"),
             resources[resource]);
    return true;
  } else {
    __asm__ volatile("" ::: "memory");
    codemarker_alpha(5); 
    __asm__ volatile("" ::: "memory");
    logEvent("FAILED deduct: insufficient %s (have %.2f, need %.2f)",
             resource == WATER
                 ? "WATER"
                 : (resource == ELECTRICITY ? "ELECTRICITY" : "FUEL"),
             resources[resource], amount);
    return false;
  }
}

/**
 * Raw addition: increases resource, logs, updates global op counter.
 */
bool addResource(int resource, double amount) {
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(6); 
  __asm__ volatile("" ::: "memory");
  if (resource < 0 || resource >= RESOURCE_COUNT || amount < 0) {
    lastError = 2;
    __asm__ volatile("" ::: "memory");
    codemarker_alpha(7); 
    __asm__ volatile("" ::: "memory");
    logEvent("ERROR: Invalid addResource call (res=%d, amt=%.2f)", resource,
             amount);
    return false;
  }
  resources[resource] += amount;
  totalOperations++;
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(8); 
  __asm__ volatile("" ::: "memory");
  logEvent("Added %.2f to %s (new level=%.2f)", amount,
           resource == WATER
               ? "WATER"
               : (resource == ELECTRICITY ? "ELECTRICITY" : "FUEL"),
           resources[resource]);
  return true;
}

/**
 * Converts from 'fromRes' to 'toRes' to obtain exactly 'targetAmount' of
 * 'toRes'. Uses global CONV_EFFICIENCY. Calls deductResource and addResource ->
 * high coupling. Returns true if conversion succeeded, false otherwise.
 */
bool convertResourceToGetTarget(int fromRes, int toRes, double targetAmount,
                                double efficiency) {
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(9); 
  __asm__ volatile("" ::: "memory");
  if (fromRes == toRes) {
    __asm__ volatile("" ::: "memory");
    codemarker_alpha(10); 
    __asm__ volatile("" ::: "memory");
    logEvent("convert: source and target are the same – no conversion needed");
    return false;
  }
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(11); 
  __asm__ volatile("" ::: "memory");
  if (targetAmount <= 0) {
    return true; /* nothing to convert */
  }

  __asm__ volatile("" ::: "memory");
  codemarker_alpha(12); 
  __asm__ volatile("" ::: "memory");
  double neededFrom = targetAmount / efficiency;
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(13); 
  __asm__ volatile("" ::: "memory");
  logEvent("Attempt conversion: %s -> %s : need %.2f of target, requires %.2f "
           "from source",
           fromRes == WATER ? "WATER"
                            : (fromRes == ELECTRICITY ? "ELECTRICITY" : "FUEL"),
           toRes == WATER ? "WATER"
                          : (toRes == ELECTRICITY ? "ELECTRICITY" : "FUEL"),
           targetAmount, neededFrom);

  /* Raw deduct from source (no recursive obtain) and raw add to target */
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(14); 
  __asm__ volatile("" ::: "memory");
  if (deductResource(fromRes, neededFrom)) {
    __asm__ volatile("" ::: "memory");
    codemarker_alpha(15); 
    __asm__ volatile("" ::: "memory");
    if (addResource(toRes, targetAmount)) {
      __asm__ volatile("" ::: "memory");
      codemarker_alpha(16); 
      __asm__ volatile("" ::: "memory");
      logEvent("Conversion successful: consumed %.2f of source, produced %.2f "
               "of target",
               neededFrom, targetAmount);
      return true;
    } else {
      /* Rollback: should never happen as addResource always succeeds for
       * non-negative */
      addResource(fromRes, neededFrom); /* restore source */
      __asm__ volatile("" ::: "memory");
      codemarker_alpha(17); 
      __asm__ volatile("" ::: "memory");
      logEvent("Conversion failed during add - rolled back");
      return false;
    }
  } else {
    __asm__ volatile("" ::: "memory");
    codemarker_alpha(18); 
    __asm__ volatile("" ::: "memory");
    logEvent("Conversion failed: insufficient source %s",
             fromRes == WATER
                 ? "WATER"
                 : (fromRes == ELECTRICITY ? "ELECTRICITY" : "FUEL"));
    return false;
  }
}

/**
 * Tries to obtain 'neededAmount' of a resource by converting from other
 * resources. Iterates through all possible sources, attempts conversion with
 * global efficiency. Highly coupled: uses convertResourceToGetTarget, logEvent,
 * totalOperations updates indirectly.
 */
bool obtainResource(int resource, double neededAmount) {
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(19); 
  __asm__ volatile("" ::: "memory");
  if (neededAmount <= 0) {
    return true;
  }
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(20); 
  __asm__ volatile("" ::: "memory");
  logEvent("ObtainResource: need %.2f of %s", neededAmount,
           resource == WATER
               ? "WATER"
               : (resource == ELECTRICITY ? "ELECTRICITY" : "FUEL"));

  /* Try each other resource as potential source */
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(21); 
  __asm__ volatile("" ::: "memory");
  for (int src = 0; src < RESOURCE_COUNT; src++) {
    __asm__ volatile("" ::: "memory");
    codemarker_alpha(22); 
    __asm__ volatile("" ::: "memory");
    if (src == resource) {
      continue;
    }
    /* Attempt conversion from src to resource */
    __asm__ volatile("" ::: "memory");
    codemarker_alpha(23); 
    __asm__ volatile("" ::: "memory");
    if (convertResourceToGetTarget(src, resource, neededAmount,
                                   CONV_EFFICIENCY)) {
      __asm__ volatile("" ::: "memory");
      codemarker_alpha(24); 
      __asm__ volatile("" ::: "memory");
      logEvent("ObtainResource succeeded via conversion from %s",
               src == WATER ? "WATER"
                            : (src == ELECTRICITY ? "ELECTRICITY" : "FUEL"));
      return true;
    }
  }
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(25); 
  __asm__ volatile("" ::: "memory");
  logEvent("ObtainResource failed: no conversion path available for %.2f of %s",
           neededAmount,
           resource == WATER
               ? "WATER"
               : (resource == ELECTRICITY ? "ELECTRICITY" : "FUEL"));
  return false;
}

/**
 * High-level use of a resource. If enough, deduct directly.
 * If not enough, tries to obtain the deficit via conversions, then deducts.
 * Deeply coupled: calls deductResource, obtainResource, logEvent.
 */
bool useResource(int resource, double amount) {
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(26); 
  __asm__ volatile("" ::: "memory");
  if (amount <= 0) {
    return true;
  }
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(27); 
  __asm__ volatile("" ::: "memory");
  logEvent("Request to use %.2f of %s", amount,
           resource == WATER
               ? "WATER"
               : (resource == ELECTRICITY ? "ELECTRICITY" : "FUEL"));

  __asm__ volatile("" ::: "memory");
  codemarker_alpha(28); 
  __asm__ volatile("" ::: "memory");
  if (resources[resource] >= amount) {
    /* Enough available – just deduct */
    return deductResource(resource, amount);
  } else {
    __asm__ volatile("" ::: "memory");
    codemarker_alpha(29); 
    __asm__ volatile("" ::: "memory");
    double deficit = amount - resources[resource];
    __asm__ volatile("" ::: "memory");
    codemarker_alpha(30); 
    __asm__ volatile("" ::: "memory");
    logEvent("Deficit of %.2f, trying to obtain additional resource", deficit);
    __asm__ volatile("" ::: "memory");
    codemarker_alpha(31); 
    __asm__ volatile("" ::: "memory");
    if (obtainResource(resource, deficit)) {
      /* Now enough should be present, deduct the whole amount */
      return deductResource(resource, amount);
    } else {
      __asm__ volatile("" ::: "memory");
      codemarker_alpha(32); 
      __asm__ volatile("" ::: "memory");
      logEvent("Cannot use %.2f of %s – insufficient after conversion attempts",
               amount,
               resource == WATER
                   ? "WATER"
                   : (resource == ELECTRICITY ? "ELECTRICITY" : "FUEL"));
      lastError = 3;
      return false;
    }
  }
}

/**
 * Produce (add) a resource. Simple raw addition, but still couples through
 * global state and logging. Also updates operation counter.
 */
bool produceResource(int resource, double amount) {
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(33); 
  __asm__ volatile("" ::: "memory");
  if (amount < 0) {
    return false;
  }
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(34); 
  __asm__ volatile("" ::: "memory");
  logEvent("Producing %.2f of %s", amount,
           resource == WATER
               ? "WATER"
               : (resource == ELECTRICITY ? "ELECTRICITY" : "FUEL"));
  return addResource(resource, amount);
}

/**
 * Prints current resource levels - uses global resources and calls logEvent.
 */
void printResources(const char *label) {
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(35); 
  __asm__ volatile("" ::: "memory");
  logEvent("=== %s ===", label);
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(36); 
  __asm__ volatile("" ::: "memory");
  printf("%s:\n", label);
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(37); 
  __asm__ volatile("" ::: "memory");
  printf("  Water:      %.2f\n", resources[WATER]);
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(38); 
  __asm__ volatile("" ::: "memory");
  printf("  Electricity:%.2f\n", resources[ELECTRICITY]);
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(39); 
  __asm__ volatile("" ::: "memory");
  printf("  Fuel:       %.2f\n", resources[FUEL]);
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(40); 
  __asm__ volatile("" ::: "memory");
  printf("  Total ops:  %d\n", totalOperations);
}

/**
 * Prints all logged events from the global buffer.
 */
void printLogs(void) {
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(41); 
  __asm__ volatile("" ::: "memory");
  printf("\n===== EVENT LOG (%d entries) =====\n", logCount);
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(42); 
  __asm__ volatile("" ::: "memory");
  for (int i = 0; i < logCount; i++) {
    __asm__ volatile("" ::: "memory");
    codemarker_alpha(43); 
    __asm__ volatile("" ::: "memory");
    printf("%s\n", logMessages[i]);
  }
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(44); 
  __asm__ volatile("" ::: "memory");
  printf("===== END OF LOG =====\n");
}

/* ---------- Main: Demonstrates High Coupling ---------- */
int main(void) {
  /* Initial resource levels */
  resources[WATER] = 100.0;
  resources[ELECTRICITY] = 50.0;
  resources[FUEL] = 80.0;
  totalOperations = 0;
  lastError = 0;
  logCount = 0;

  __asm__ volatile("" ::: "memory");
  codemarker_alpha(45); 
  __asm__ volatile("" ::: "memory");
  printResources("Initial State");

  /* DEMO 1: Use electricity beyond current level – triggers conversion from
   * fuel */
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(46); 
  __asm__ volatile("" ::: "memory");
  printf("\n--- Demo 1: Heavy electricity usage (needs conversion) ---\n");
  useResource(
      ELECTRICITY,
      70.0); /* Only 50 available, needs 20 more -> conversion from fuel */

  /* DEMO 2: Produce extra fuel, then use water with conversion */
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(47); 
  __asm__ volatile("" ::: "memory");
  printf("\n--- Demo 2: Produce fuel and convert water to electricity ---\n");
  produceResource(FUEL, 30.0);
  convertResourceToGetTarget(WATER, ELECTRICITY, 15.0, CONV_EFFICIENCY);

  /* DEMO 3: Massive water usage that will fail partially */
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(48); 
  __asm__ volatile("" ::: "memory");
  printf("\n--- Demo 3: Massive water consumption (beyond reserves and "
         "conversion limits) ---\n");
  useResource(
      WATER,
      120.0); /* May try to convert from fuel/electricity but likely fails */

  /* DEMO 4: Mixed operations – high coupling in action */
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(49); 
  __asm__ volatile("" ::: "memory");
  printf("\n--- Demo 4: Interleaved production and usage ---\n");
  produceResource(ELECTRICITY, 10.0);
  useResource(FUEL, 50.0);
  produceResource(WATER, 40.0);
  useResource(ELECTRICITY, 25.0);

  /* Final state and logs */
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(50); 
  __asm__ volatile("" ::: "memory");
  printResources("Final State");
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(51); 
  __asm__ volatile("" ::: "memory");
  printf("\nLast error code: %d\n", lastError);
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(52); 
  __asm__ volatile("" ::: "memory");
  printLogs();

  /* Analysis of coupling: every function above shared global resources,
   * logs, and counters. Changing conversion efficiency or resource array
   * would break multiple functions. The circular flow
   * (use->obtain->convert->deduct) makes it impossible to change one without
   * affecting others.
   */
  return 0;
}
