using System;
using System.Collections.Generic;
using System.Reflection;
using Xunit;
using HT9045.Core;          // forces the Core assembly to be loaded into the AppDomain
using HT9045.Hardware.Sim;  // forces Sim assembly to load (so its name is resolvable)
using HT9045.Infrastructure; // same for Infrastructure

namespace HT9045.Tests
{
    // Reflection-based architecture guard: asserts that HT9045.Core does NOT
    // take a compile-time dependency on any of the forbidden implementation
    // assemblies. This enforces the ports-and-adapters layering — Core must
    // depend only on HardwareAbstraction, Presentation.Abstraction and the BCL.
    //
    // Forbidden assembly name prefixes (lower-invariant):
    //   ht9045.hardware.sim
    //   ht9045.hardware.native
    //   ht9045.ui.winforms         (Phase 4 — must not leak into Core)
    //   ht9045.infrastructure
    public class ArchitectureTests
    {
        // The HT9045.Core assembly, obtained from a type we know lives in it.
        private static readonly Assembly _coreAssembly =
            typeof(HandlerOrchestrator).Assembly;

        // Assembly names that Core must never reference.
        private static readonly string[] _forbidden = new string[]
        {
            "ht9045.hardware.sim",
            "ht9045.hardware.native",
            "ht9045.ui.winforms",
            "ht9045.infrastructure",
        };

        [Fact]
        public void Core_DoesNotReference_HardwareSim()
        {
            AssertCoreDoesNotReference("ht9045.hardware.sim");
        }

        [Fact]
        public void Core_DoesNotReference_HardwareNative()
        {
            AssertCoreDoesNotReference("ht9045.hardware.native");
        }

        [Fact]
        public void Core_DoesNotReference_UiWinForms()
        {
            AssertCoreDoesNotReference("ht9045.ui.winforms");
        }

        [Fact]
        public void Core_DoesNotReference_Infrastructure()
        {
            AssertCoreDoesNotReference("ht9045.infrastructure");
        }

        [Fact]
        public void Core_DoesReference_HardwareAbstraction()
        {
            // Positive control: Core MUST reference HardwareAbstraction.
            bool found = false;
            AssemblyName[] refs = _coreAssembly.GetReferencedAssemblies();
            for (int i = 0; i < refs.Length; i++)
            {
                if (refs[i].Name.ToLowerInvariant() == "ht9045.hardwareabstraction")
                {
                    found = true;
                    break;
                }
            }
            Assert.True(found,
                "HT9045.Core should reference HT9045.HardwareAbstraction but does not. " +
                "Referenced: " + FormatRefs(refs));
        }

        [Fact]
        public void Core_DoesReference_PresentationAbstraction()
        {
            // Positive control: Core MUST reference Presentation.Abstraction.
            bool found = false;
            AssemblyName[] refs = _coreAssembly.GetReferencedAssemblies();
            for (int i = 0; i < refs.Length; i++)
            {
                if (refs[i].Name.ToLowerInvariant() == "ht9045.presentation.abstraction")
                {
                    found = true;
                    break;
                }
            }
            Assert.True(found,
                "HT9045.Core should reference HT9045.Presentation.Abstraction but does not. " +
                "Referenced: " + FormatRefs(refs));
        }

        // ------------------------------------------------------------------ helpers

        private static void AssertCoreDoesNotReference(string forbiddenNameLower)
        {
            AssemblyName[] refs = _coreAssembly.GetReferencedAssemblies();
            for (int i = 0; i < refs.Length; i++)
            {
                string name = refs[i].Name.ToLowerInvariant();
                if (name == forbiddenNameLower || name.StartsWith(forbiddenNameLower))
                {
                    throw new Xunit.Sdk.XunitException(
                        "HT9045.Core must NOT reference '" + refs[i].Name + "' " +
                        "(ports-and-adapters: Core depends only on abstraction assemblies). " +
                        "All referenced assemblies: " + FormatRefs(refs));
                }
            }
            // No forbidden reference found — test passes.
        }

        private static string FormatRefs(AssemblyName[] refs)
        {
            List<string> names = new List<string>();
            for (int i = 0; i < refs.Length; i++)
                names.Add(refs[i].Name);
            return "[" + string.Join(", ", names) + "]";
        }
    }
}
