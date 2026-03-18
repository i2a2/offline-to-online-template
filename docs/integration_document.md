# Integration Document

<!--
  Template version: 1.0.0
  Based on: "Methodology for Offline-to-Online Migration of Diagnostic Processing
             in Fusion I&C Environments" (Ch. 3)

  Usage:
    - Copy this file into your project repository.
    - Fill in each section as the workflow progresses.
    - Commit after each gate closure with a meaningful message.
    - Fields marked with `(*)` are mandatory for gate closure.
    - Fields marked with `TBD` are placeholders to be replaced.
    - Use `N/A` when a field does not apply to your case.

  Versioning convention:
    This document is versioned implicitly through git history.
    The "Revision" field below tracks the methodology-level version
    (v0 after Step 1, v1 after Step 2, ..., final after Step 10).
-->


## 0. Document metadata

| Field              | Value                                          |
|--------------------|------------------------------------------------|
| Project            | TBD                                            |
| Diagnostic         | TBD                                            |
| Facility           | TBD                                            |
| Author(s)          | TBD                                            |
| Created            | YYYY-MM-DD                                     |
| Revision           | v0                                             |
| Repository         | TBD (URL or path)                              |
| Offline reference  | TBD (language, repository path, commit/tag)     |
| Target platform    | TBD (board, device, execution stack)            |
| I&C environment    | TBD (RTF / DCS / other)                         |


---


## 1. Feasibility analysis

**Gate:** Feasibility verdict  
**Status:** `NOT STARTED` | `IN PROGRESS` | `CLOSED` | `BLOCKED`

### 1.1 Diagnostic role in the acquisition chain

TBD — Describe the diagnostic, its role, and the signals it produces.

### 1.2 Operational interface

| Parameter                   | Value       |
|-----------------------------|-------------|
| Input signal(s)             | TBD         |
| Output signal(s)            | TBD         |
| Input data rate             | TBD         |
| Invocation rate             | TBD         |
| Samples per event           | TBD         |
| Intended consumers          | TBD         |

### 1.3 Timing budget

| Parameter                   | Value       |
|-----------------------------|-------------|
| End-to-end latency bound    | TBD         |
| Admissible jitter           | TBD         |
| Source of timing requirement | TBD         |

### 1.4 Offline-only dependencies and blockers

TBD — List any identified offline-only dependencies (multi-pass processing,
file access, interactive workflows, database lookups at runtime, etc.)
and assess whether each can be resolved or constitutes a blocker.

### 1.5 Feasibility verdict (*)

| Field                       | Value                                        |
|-----------------------------|----------------------------------------------|
| Verdict                     | `PROCEED` / `DEMONSTRATOR` / `DEFER` / `REJECT` |
| Rationale                   | TBD                                          |
| Demonstrator scope (if applicable) | TBD                                   |
| Blockers (if applicable)    | TBD                                          |
| Prerequisites for next step | TBD                                          |

### 1.6 Evidence and references

- [ ] Integration document v0 committed


---


## 2. High-level refactoring

**Gate:** Two-block decomposition defined  
**Status:** `NOT STARTED` | `IN PROGRESS` | `CLOSED` | `BLOCKED`

### 2.1 Dependency inventory

TBD — List all dependencies required to reproduce offline outputs
(auxiliary files, calibration databases, configuration parameters,
intermediate computations, external libraries, etc.).

### 2.2 Signal-path block definition (experiment-time)

| Field                       | Value       |
|-----------------------------|-------------|
| Description                 | TBD         |
| Inputs (per event)          | TBD         |
| Outputs (per event)         | TBD         |
| State requirements          | TBD         |
| Pre-experiment consumables  | TBD         |

### 2.3 Configuration and calibration supporting block definition (pre-experiment)

| Field                       | Value       |
|-----------------------------|-------------|
| Description                 | TBD         |
| Inputs (sources)            | TBD         |
| Outputs (artifacts for signal-path block) | TBD |
| Execution frequency         | TBD (per-shot / per-campaign / on-change) |

### 2.4 Decisions and open issues

TBD — Record any ambiguities, boundary decisions, or assumptions
made during the decomposition.

### 2.5 Evidence and references

- [ ] Signal-path block specification documented
- [ ] Supporting block specification documented
- [ ] Integration document updated to v1


---


## 3. Configuration and calibration handling

**Gate:** Synthesis-ready artifacts with provenance  
**Status:** `NOT STARTED` | `IN PROGRESS` | `CLOSED` | `BLOCKED`

### 3.1 Parameter schema

| Parameter name | Type   | Unit | Range / domain | Source          |
|----------------|--------|------|----------------|-----------------|
| TBD            | TBD    | TBD  | TBD            | TBD             |

### 3.2 Preprocessing utility

| Field                       | Value       |
|-----------------------------|-------------|
| Language / tool              | TBD         |
| Repository path             | TBD         |
| Input sources               | TBD         |
| Output artifact format      | TBD (e.g., C/C++ header files) |

### 3.3 Provenance metadata

| Field                       | Value       |
|-----------------------------|-------------|
| Generator identity / version | TBD        |
| Source identifiers           | TBD (shot, calibration campaign, etc.) |
| Repository state at generation | TBD (commit hash) |
| Generation timestamp         | TBD        |

### 3.4 Regeneration rule

TBD — Define how to distinguish operational changes (new calibration data)
from generator evolution (code corrections, model refinements),
and how each triggers artifact regeneration.

### 3.5 Evidence and references

- [ ] Synthesis-ready artifact set generated and committed
- [ ] Provenance header embedded in artifacts
- [ ] Preprocessing utility documented and committed
- [ ] Integration document updated to v2


---


## 4. Partitioning

**Gate:** Partitioning report issued  
**Status:** `NOT STARTED` | `IN PROGRESS` | `CLOSED` | `BLOCKED`

### 4.1 Functional stage catalog

<!--
  List each functional stage in the processing chain.
  Add or remove rows as needed.
-->

| Stage | Name   | Responsibility   | Inputs      | Outputs     | Artifact deps |
|-------|--------|------------------|-------------|-------------|---------------|
| 1     | TBD    | TBD              | TBD         | TBD         | TBD           |
| 2     | TBD    | TBD              | TBD         | TBD         | TBD           |

### 4.2 Datapath diagram

TBD — Reference a diagram (file path in repo) or describe the
stage ordering and data flow between stages.

### 4.3 Partitioning decision

| Field                       | Value       |
|-----------------------------|-------------|
| Partitioning option         | TBD (single kernel / multi-kernel / hybrid) |
| Rationale                   | TBD         |
| Host participation required | YES / NO    |
| Host participation scope    | TBD (if YES) |

### 4.4 Partitioning criteria applied

<!--
  Check the criteria that were evaluated during partitioning.
-->

- [ ] Global memory traffic follows load-compute-store pattern
- [ ] Boundaries placed at streamable function interfaces
- [ ] Intra-kernel decomposition exhausted before multi-kernel split
- [ ] Multiple kernels justified by parallelism/replication gain
- [ ] Pipeline/dataflow constraints respected (no feedback deps)
- [ ] Profiling confirms compute-bound vs. data-movement-bound

### 4.5 Evidence and references

- [ ] Datapath specification committed
- [ ] Partitioning decision record committed
- [ ] Host participation statement documented (if applicable)
- [ ] Integration document updated to v3


---


## 5. Component implementation

**Gate:** Buildable baseline implementation  
**Status:** `NOT STARTED` | `IN PROGRESS` | `CLOSED` | `BLOCKED`

### 5.1 HLS top-level interface

| Port name  | Direction | Type / width | Description              |
|------------|-----------|--------------|--------------------------|
| TBD        | TBD       | TBD          | TBD                      |

### 5.2 State and reset behavior

| Field                       | Value       |
|-----------------------------|-------------|
| Persistent state            | TBD (describe what survives across events) |
| Reset trigger               | TBD (describe what causes state reset) |
| Initialization requirements | TBD         |

### 5.3 Build manifest

| Field                       | Value       |
|-----------------------------|-------------|
| HLS tool and version        | TBD         |
| Target device / part        | TBD         |
| Target clock period         | TBD         |
| Source files (repo path)    | TBD         |
| Build command               | TBD         |

### 5.4 Initial hotspot and risk log

| ID  | Description              | Severity | Status   |
|-----|--------------------------|----------|----------|
| H01 | TBD                      | TBD      | OPEN / RESOLVED |

### 5.5 Evidence and references

- [ ] Synthesizable HLS baseline committed
- [ ] Build manifest committed
- [ ] Hotspot/risk log initialized
- [ ] Integration document updated to v4


---


## 6. Component functional equivalence verification

**Gate:** Verified implementation  
**Status:** `NOT STARTED` | `IN PROGRESS` | `CLOSED` | `BLOCKED`

### 6.1 Testbench specification

| Field                       | Value       |
|-----------------------------|-------------|
| Reference model             | TBD (path, language, commit/tag) |
| Test vector source          | TBD         |
| Number of test cases        | TBD         |
| Expanded dataset (if applicable) | TBD    |

### 6.2 Tolerance definitions

| Output quantity  | Tolerance type       | Threshold   |
|------------------|----------------------|-------------|
| TBD              | abs/rel              | abs_tol=TBD, rel_tol=TBD |
| TBD              | log-ratio            | lr_tol=TBD  |

### 6.3 Functional equivalence results (*)

| Output quantity  | Worst-case deviation | Pass/Fail |
|------------------|----------------------|-----------|
| TBD              | TBD                  | TBD       |

### 6.4 HLS C-synthesis summary

| Metric           | Value       |
|------------------|-------------|
| Estimated latency (cycles) | TBD |
| Initiation interval (II)   | TBD |
| Clock period achieved       | TBD |
| BRAM usage        | TBD        |
| DSP usage         | TBD        |
| FF usage          | TBD        |
| LUT usage         | TBD        |

### 6.5 Refinement log

| Iteration | Change description       | Impact on latency | Impact on resources | Functional equiv. preserved |
|-----------|--------------------------|-------------------|---------------------|-----------------------------|
| 1         | TBD                      | TBD               | TBD                 | YES / NO                    |

### 6.6 Evidence and references

- [ ] Verification testbench committed
- [ ] Functional equivalence report produced
- [ ] HLS C-synthesis reports archived
- [ ] Hotspot/risk log updated
- [ ] Integration document updated to v5


---


## 7. Platform-specific deployment

**Gate:** Deployable package  
**Status:** `NOT STARTED` | `IN PROGRESS` | `CLOSED` | `BLOCKED`

### 7.1 Deployment route

| Field                       | Value       |
|-----------------------------|-------------|
| Route                       | TBD (XRT kernel / IP integration / vendor DAQ / other) |
| Target BSP / platform project | TBD       |
| Target device               | TBD         |

### 7.2 Post-implementation results

| Metric           | Value       | Budget     |
|------------------|-------------|------------|
| Clock achieved   | TBD         | TBD        |
| BRAM usage       | TBD         | TBD        |
| DSP usage        | TBD         | TBD        |
| FF usage         | TBD         | TBD        |
| LUT usage        | TBD         | TBD        |

### 7.3 Platform deliverables

| Deliverable      | Path in repository       |
|------------------|--------------------------|
| Bitstream / xclbin / image | TBD              |
| Build script     | TBD                      |
| Deployment record | TBD                     |

### 7.4 Deployment procedure

TBD — Document the exact steps to load and initialize the deliverable
on the target platform. This section should be sufficient for another
engineer to reproduce the deployment.

### 7.5 Evidence and references

- [ ] Platform executables generated and committed (or archived with hash)
- [ ] Deployment and initialization record committed
- [ ] Post-implementation resource/timing report archived
- [ ] Integration document updated to v5 (deployment addendum)


---


## 8. Host-side orchestration

**Gate:** Runnable host baseline  
**Status:** `NOT STARTED` | `IN PROGRESS` | `CLOSED` | `BLOCKED`

### 8.1 Host responsibilities

TBD — Define what the host does and explicitly state what is out of scope
for the host layer.

### 8.2 Per-event interface

| Direction | Signal / buffer | Type / size | Description              |
|-----------|----------------|-------------|--------------------------|
| IN        | TBD            | TBD         | TBD                      |
| OUT       | TBD            | TBD         | TBD                      |

### 8.3 Lifecycle management

| Phase        | Action                                    |
|--------------|-------------------------------------------|
| Initialize   | TBD                                       |
| Start        | TBD                                       |
| Per-event    | TBD                                       |
| Stop         | TBD                                       |
| Reset        | TBD                                       |

### 8.4 Observability and diagnostics

| Signal / metric  | Description              | Collection method |
|------------------|--------------------------|-------------------|
| TBD              | TBD                      | TBD               |

### 8.5 Build and run manifest

| Field                       | Value       |
|-----------------------------|-------------|
| Host executable path        | TBD         |
| Build command               | TBD         |
| Run command                 | TBD         |
| Runtime dependencies        | TBD         |

### 8.6 Evidence and references

- [ ] Host orchestration layer committed
- [ ] Host execution record produced
- [ ] Operating guide committed
- [ ] Integration document updated to v6


---


## 9. I&C integration

<!--
  Fill in section 9a OR 9b (or both) depending on the target I&C route.
  Delete the section that does not apply, or keep both if multiple
  routes are being evaluated.
-->

### 9a. RTF integration route

**Gate:** Integrated RTF application  
**Status:** `NOT STARTED` | `IN PROGRESS` | `CLOSED` | `BLOCKED` | `N/A`

#### 9a.1 RTF application structure

| Field                       | Value       |
|-----------------------------|-------------|
| Function Block name(s)      | TBD         |
| Signal I/O mapping          | TBD         |
| Process placement            | TBD         |

#### 9a.2 Non-real-time initialization

TBD — Describe hardware management actions bound to the RTF configure phase
(binary loading, buffer allocation, one-time parameter transfers).

#### 9a.3 Real-time process loop

TBD — Describe the per-cycle sequence: read inputs, transfer data,
trigger execution, collect results, publish outputs.

#### 9a.4 Determinism-critical runtime settings

| Setting                     | Value       |
|-----------------------------|-------------|
| CPU core isolation          | TBD         |
| Thread affinity             | TBD         |
| Scheduling policy           | TBD         |
| IRQ affinity                | TBD         |

#### 9a.5 Evidence and references

- [ ] RTF application committed
- [ ] Integration and timing record produced
- [ ] Integration document updated to v7

---

### 9b. DCS integration route

**Gate:** Integrated DCS application  
**Status:** `NOT STARTED` | `IN PROGRESS` | `CLOSED` | `BLOCKED` | `N/A`

#### 9b.1 DCS application structure

| Field                       | Value       |
|-----------------------------|-------------|
| DCS-facing I/O points       | TBD         |
| Event input binding         | TBD         |
| Configuration input binding | TBD         |
| Published output binding    | TBD         |

#### 9b.2 Non-real-time initialization

TBD — Describe FPGA image loading, runtime stack preparation,
buffer allocation, and one-time parameter transfers.

#### 9b.3 Online execution cycle

TBD — Describe the per-cycle sequence in DCS context: acquire/receive inputs,
trigger execution, collect results, publish outputs.

#### 9b.4 Determinism-critical runtime settings

| Setting                     | Value       |
|-----------------------------|-------------|
| CPU core isolation          | TBD         |
| Thread affinity             | TBD         |
| Scheduling policy           | TBD         |
| IRQ affinity                | TBD         |

#### 9b.5 Evidence and references

- [ ] DCS-integrated application committed
- [ ] Integration and timing record (DCS) produced
- [ ] Integration document updated to v7


---


## 10. System-level validation and acceptance

**Gate:** Accepted deployment baseline  
**Status:** `NOT STARTED` | `IN PROGRESS` | `CLOSED` | `BLOCKED`

### 10.1 Acceptance protocol definition

#### Functional tolerances

| Output quantity  | Tolerance type | Threshold   |
|------------------|----------------|-------------|
| TBD              | abs/rel        | abs_tol=TBD, rel_tol=TBD |
| TBD              | log-ratio      | lr_tol=TBD  |

#### Timing targets

| Parameter                   | Target      |
|-----------------------------|-------------|
| End-to-end latency bound    | TBD         |
| Admissible jitter (indicator) | TBD       |
| Sustained throughput         | TBD         |

#### Observability signals

| Signal / metric  | Measurement point        | Collection method |
|------------------|--------------------------|-------------------|
| TBD              | TBD                      | TBD               |

### 10.2 Acceptance run conditions (*)

| Field                       | Value       |
|-----------------------------|-------------|
| Dataset used                | TBD (path, shot IDs, description) |
| Number of events processed  | TBD         |
| Execution mode              | TBD (replay / live / on-card deterministic) |
| Host configuration          | TBD (CPU, OS, kernel version, RT patches) |
| CPU core assignment         | TBD         |
| Scheduling policy           | TBD         |
| Tracing / profiling level   | TBD         |

### 10.3 Functional verification results (*)

| Output quantity  | Worst-case |x_impl - x_ref| | Tolerance | Pass/Fail |
|------------------|-------------------------------|-----------|-----------|
| TBD              | TBD                           | TBD       | TBD       |

Deviations and notes:

TBD — Record any observed deviations, edge cases, or anomalies.

### 10.4 Timing results (*)

| Metric                         | Value       |
|--------------------------------|-------------|
| Mean latency                   | TBD         |
| Median latency                 | TBD         |
| P95 latency                    | TBD         |
| P99 latency                    | TBD         |
| Max latency                    | TBD         |
| Jitter indicator               | TBD         |
| Sustained throughput           | TBD         |
| Backlog observed               | YES / NO    |
| Number of events in campaign   | TBD         |

### 10.5 Acceptance verdict (*)

| Field                       | Value       |
|-----------------------------|-------------|
| Functional compliance       | `PASS` / `FAIL` / `CONDITIONAL` |
| Timing compliance           | `PASS` / `FAIL` / `CONDITIONAL` |
| Overall verdict             | `ACCEPTED` / `REJECTED` / `CONDITIONAL` |
| Conditions (if conditional) | TBD         |

### 10.6 Accepted baseline freeze (*)

| Field                       | Value       |
|-----------------------------|-------------|
| Baseline tag / release ID   | TBD         |
| Bitstream / xclbin hash     | TBD         |
| Host executable hash        | TBD         |
| Synthesis-ready artifact hash | TBD       |
| Acceptance report path      | TBD         |
| Date frozen                 | YYYY-MM-DD  |

### 10.7 Evidence and references

- [ ] Acceptance report committed
- [ ] Baseline release tagged in repository
- [ ] Integration document finalized (final revision)


---


## Changelog

<!--
  Track major updates to this document.
  Each entry should correspond to a gate closure or a significant revision.
  For fine-grained history, rely on git log.
-->

| Date       | Revision | Step closed | Author | Summary                     |
|------------|----------|-------------|--------|-----------------------------|
| YYYY-MM-DD | v0       | Step 1      | TBD    | Feasibility verdict issued  |
