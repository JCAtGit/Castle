extern void codemarker_alpha(int markerID);
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

#define CONV_EFFICIENCY 0.8 /* Conversion loss: 80% of source becomes target   \
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
  codemarker_alpha(0); 
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
  codemarker_alpha(1); 
  if (resource < 0 || resource >= RESOURCE_COUNT || amount < 0) {
    lastError = 1;
    codemarker_alpha(2); 
    logEvent("ERROR: Invalid deductResource call (res=%d, amt=%.2f)", resource,
             amount);
    return false;
  }
  codemarker_alpha(3); 
  if (resources[resource] >= amount) {
    resources[resource] -= amount;
    totalOperations++;
    codemarker_alpha(4); 
    logEvent("Deducted %.2f from %s (new level=%.2f)", amount,
             resource == WATER
                 ? "WATER"
                 : (resource == ELECTRICITY ? "ELECTRICITY" : "FUEL"),
             resources[resource]);
    return true;
  } else {
    codemarker_alpha(5); 
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
  codemarker_alpha(6); 
  if (resource < 0 || resource >= RESOURCE_COUNT || amount < 0) {
    lastError = 2;
    codemarker_alpha(7); 
    logEvent("ERROR: Invalid addResource call (res=%d, amt=%.2f)", resource,
             amount);
    return false;
  }
  resources[resource] += amount;
  totalOperations++;
  codemarker_alpha(8); 
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
  codemarker_alpha(9); 
  if (fromRes == toRes) {
    codemarker_alpha(10); 
    logEvent("convert: source and target are the same – no conversion needed");
    return false;
  }
  codemarker_alpha(11); 
  if (targetAmount <= 0) {
    return true; /* nothing to convert */
  }

  codemarker_alpha(12); 
  double neededFrom = targetAmount / efficiency;
  codemarker_alpha(13); 
  logEvent("Attempt conversion: %s -> %s : need %.2f of target, requires %.2f "
           "from source",
           fromRes == WATER ? "WATER"
                            : (fromRes == ELECTRICITY ? "ELECTRICITY" : "FUEL"),
           toRes == WATER ? "WATER"
                          : (toRes == ELECTRICITY ? "ELECTRICITY" : "FUEL"),
           targetAmount, neededFrom);

  /* Raw deduct from source (no recursive obtain) and raw add to target */
  codemarker_alpha(14); 
  if (deductResource(fromRes, neededFrom)) {
    codemarker_alpha(15); 
    if (addResource(toRes, targetAmount)) {
      codemarker_alpha(16); 
      logEvent("Conversion successful: consumed %.2f of source, produced %.2f "
               "of target",
               neededFrom, targetAmount);
      return true;
    } else {
      /* Rollback: should never happen as addResource always succeeds for
       * non-negative */
      addResource(fromRes, neededFrom); /* restore source */
      codemarker_alpha(17); 
      logEvent("Conversion failed during add - rolled back");
      return false;
    }
  } else {
    codemarker_alpha(18); 
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
  codemarker_alpha(19); 
  if (neededAmount <= 0) {
    return true;
  }
  codemarker_alpha(20); 
  logEvent("ObtainResource: need %.2f of %s", neededAmount,
           resource == WATER
               ? "WATER"
               : (resource == ELECTRICITY ? "ELECTRICITY" : "FUEL"));

  /* Try each other resource as potential source */
  codemarker_alpha(21); 
  for (int src = 0; src < RESOURCE_COUNT; src++) {
    codemarker_alpha(22); 
    if (src == resource) {
      continue;
    }
    /* Attempt conversion from src to resource */
    codemarker_alpha(23); 
    if (convertResourceToGetTarget(src, resource, neededAmount,
                                   CONV_EFFICIENCY)) {
      codemarker_alpha(24); 
      logEvent("ObtainResource succeeded via conversion from %s",
               src == WATER ? "WATER"
                            : (src == ELECTRICITY ? "ELECTRICITY" : "FUEL"));
      return true;
    }
  }
  codemarker_alpha(25); 
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
  codemarker_alpha(26); 
  if (amount <= 0) {
    return true;
  }
  codemarker_alpha(27); 
  logEvent("Request to use %.2f of %s", amount,
           resource == WATER
               ? "WATER"
               : (resource == ELECTRICITY ? "ELECTRICITY" : "FUEL"));

  codemarker_alpha(28); 
  if (resources[resource] >= amount) {
    /* Enough available – just deduct */
    return deductResource(resource, amount);
  } else {
    codemarker_alpha(29); 
    double deficit = amount - resources[resource];
    codemarker_alpha(30); 
    logEvent("Deficit of %.2f, trying to obtain additional resource", deficit);
    codemarker_alpha(31); 
    if (obtainResource(resource, deficit)) {
      /* Now enough should be present, deduct the whole amount */
      return deductResource(resource, amount);
    } else {
      codemarker_alpha(32); 
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
  codemarker_alpha(33); 
  if (amount < 0) {
    return false;
  }
  codemarker_alpha(34); 
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
  codemarker_alpha(35); 
  logEvent("=== %s ===", label);
  codemarker_alpha(36); 
  printf("%s:\n", label);
  codemarker_alpha(37); 
  printf("  Water:      %.2f\n", resources[WATER]);
  codemarker_alpha(38); 
  printf("  Electricity:%.2f\n", resources[ELECTRICITY]);
  codemarker_alpha(39); 
  printf("  Fuel:       %.2f\n", resources[FUEL]);
  codemarker_alpha(40); 
  printf("  Total ops:  %d\n", totalOperations);
}

/**
 * Prints all logged events from the global buffer.
 */
void printLogs(void) {
  codemarker_alpha(41); 
  printf("\n===== EVENT LOG (%d entries) =====\n", logCount);
  codemarker_alpha(42); 
  for (int i = 0; i < logCount; i++) {
    codemarker_alpha(43); 
    printf("%s\n", logMessages[i]);
  }
  codemarker_alpha(44); 
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

  codemarker_alpha(45); 
  printResources("Initial State");

  /* DEMO 1: Use electricity beyond current level – triggers conversion from
   * fuel */
  codemarker_alpha(46); 
  printf("\n--- Demo 1: Heavy electricity usage (needs conversion) ---\n");
  useResource(
      ELECTRICITY,
      70.0); /* Only 50 available, needs 20 more -> conversion from fuel */

  /* DEMO 2: Produce extra fuel, then use water with conversion */
  codemarker_alpha(47); 
  printf("\n--- Demo 2: Produce fuel and convert water to electricity ---\n");
  produceResource(FUEL, 30.0);
  convertResourceToGetTarget(WATER, ELECTRICITY, 15.0, CONV_EFFICIENCY);

  /* DEMO 3: Massive water usage that will fail partially */
  codemarker_alpha(48); 
  printf("\n--- Demo 3: Massive water consumption (beyond reserves and "
         "conversion limits) ---\n");
  useResource(
      WATER,
      120.0); /* May try to convert from fuel/electricity but likely fails */

  /* DEMO 4: Mixed operations – high coupling in action */
  codemarker_alpha(49); 
  printf("\n--- Demo 4: Interleaved production and usage ---\n");
  produceResource(ELECTRICITY, 10.0);
  useResource(FUEL, 50.0);
  produceResource(WATER, 40.0);
  useResource(ELECTRICITY, 25.0);

  /* Final state and logs */
  codemarker_alpha(50); 
  printResources("Final State");
  codemarker_alpha(51); 
  printf("\nLast error code: %d\n", lastError);
  codemarker_alpha(52); 
  printLogs();

  /* Analysis of coupling: every function above shared global resources,
   * logs, and counters. Changing conversion efficiency or resource array
   * would break multiple functions. The circular flow
   * (use->obtain->convert->deduct) makes it impossible to change one without
   * affecting others.
   */
  return 0;
}
