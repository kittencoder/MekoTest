#ifndef MEKO_H
#define MEKO_H

#include "externals.h"

#if __has_include("../MekoConfig.h")
#include "../MekoConfig.h"
#else
// if not found, include the template file that enables EVERYTHING by default...
// this way everything willl work, and the config file is used as a "I don't need that code"-flag for more efficient compilation and smaller output files
#include "MekoConfig.h"
#endif
// process set config values
#include "MekoConfigProcessor.h"

#include "definitions.h"

// ///// //
// ///// // Generic classes
// ///// //
#include "Support/non_copyable.h"
#include "Support/Singleton.h"
#include "Support/StringUtilities.h"
#include "Support/TimeUtilities.h"
#include "Color/Style.h"
#include "Support/TablePrinter.h"

// ///// //
// ///// // Async operations
// ///// //
#include "Exec/ExitCleanup.h"
#include "Exec/CommPipe.h"
#include "Exec/Run.h"
#include "Exec/RunProcess.h"
#include "Exec/Shell.h"

// ///// //
// ///// // Testsuite
// ///// //
#include "TestSuite/MekoTestSuiteInc.h"

#endif