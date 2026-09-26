# Claude.ai Archive Interaction Guide

## Purpose & Scope

The material under `docs/prj/claude/` is a local archive exported from the
Claude.ai project where substantial portions of Chevron's design were explored
and developed. It preserves historical reasoning, design evolution, discussions,
intermediate artifacts, and decisions that may help explain how Chevron reached
its current state.

This guide exists to help agents retrieve and interpret that historical context
efficiently. It describes how the exported material is organized, how to locate
relevant information, how to reason about chronology and design state, and how
to avoid known archive-specific failure modes.

**The archive is historical evidence, not Chevron's canonical specification**.
It may provide important context for understanding *why* a design exists or
*how* it evolved, but **it represents Chevron at particular points in its
development and must be interpreted accordingly**.

**The exported archive itself is immutable. Agents must not modify, rewrite,
delete, rename, or otherwise alter its historical contents**. Supporting
Chevron-authored material stored alongside the archive, including this guide,
is not part of that restriction.

This guide applies only to the locally stored Claude.ai exports. It does not
describe interaction with the live Claude.ai project, general repository
exploration, or Chevron documentation outside this archive.

</br>

## Archive Layout

The Claude.ai archive is composed of **three primary sources of historical
project context**. Each serves a different role and should be interpreted
accordingly.

| Archive Material                      | Role                                                                                                                                                                                                                                                                                                                     |
| ------------------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ |
| `claude_chevron_project_memory.md`    | Historical project memory autonomously maintained by Claude.ai while the Chevron project was active. It provides broad orientation on the project's state, principles, active concerns, and development direction at the time it was maintained. It is frozen historical material, not a living project-status document. |
| `claude_chevron_library_project.json` | Export of the Claude.ai project itself. It contains project metadata together with embedded Chevron project and design documents that were available as project context during the archived work. The embedded documents are the practically useful portion for most investigations.                                     |
| `conversations/`                      | Full exported design conversations. These preserve the reasoning, proposals, disagreements, revisions, and decisions that developed around Chevron over time. They may also contain intermediate artifacts and historical document snapshots that do not exist elsewhere in the archive.                                 |

**The embedded project documents and conversation history are closely
related**. Many of the documents were produced or revised through the archived
design sessions, while earlier versions or intermediate forms may remain
preserved inside conversation attachments or tool-generated content.

**The documents stored in the project export should therefore be treated as the
final versions preserved by that export, not necessarily as the final state of
the design itself**. Later conversation material may contain revisions,
clarifications, or decisions that were never propagated back into those
documents.

</br>

## Conversation Structure

Each exported conversation is stored as a JSON array containing a single
conversation object. The conversation record contains metadata, a generated
summary, and the complete message history under `chat_messages`.

### Conversation Record
The fields most useful for archive research are:

| Field                       | Purpose
| ----------------------------|---------------------------------------------------------- |
| `name`                      | Identifies the conversation.                              |
| `created_at` / `updated_at` | Bounds the conversation activity period.                  |
| `summary`                   | Claude.ai Rolling Auto-Summary preserved with the export. |
| `chat_messages`             | Exported message history.                                 |

Other conversation-level metadata exists but generally has little value when
retrieving historical design context.

### Message Content
Each message contains metadata together with **two important representations of
its contents**: `text` and `content`.

`text` is convenient for broad searching and lightweight inspection, **but it is
not a clean representation of only the *visible* conversational response**.
Depending on the message, **it may include preserved reasoning before the
response**, while unsupported structured blocks may instead appear only as
placeholder text.

**`content` is the structured, full-fidelity representation of a message**. It
keeps ordinary conversational `text` blocks separate from `thinking`, tool
activity, and other structured material. **When the actual response text matters,
prefer the `text` blocks inside `content` rather than assuming the message-level
`text` field contains *only* the response**.

The archive contains several useful block types:

| Block         | Contents
| --------------|--------------------------------------------------------------------------- |
| `text`        | Ordinary conversational text.                                              |
| `tool_use`    | Tool invocations and their inputs.                                         |
| `tool_result` | Material returned by tools and possibly additional structured content.     |
| `thinking`    | Preserved reasoning content under the `thinking` field rather than `text`. |

Use `text` when its representation is sufficient for the investigation. Inspect
`content` when a message contains placeholders, tool activity, generated
artifacts, or other material requiring full-fidelity access.

**User messages are not labeled consistently across all conversation exports**.
The `sender` field may identify the user as either `Jamon` or `human`, so
cross-conversation filtering must account for both values.

Messages may also contain `files` and `attachments`. `files` entries are
references to Claude.ai sandbox material and should not be assumed to resolve to
other files in this archive. Attachments may contain `extracted_content`,
including historical document snapshots or other material that no longer exists
elsewhere in the archive.

</br>

## Building Context from the Archive

**Do not begin archive research by reading complete conversation transcripts.
Build context progressively, starting with broad orientation and narrowing only
as the task requires**.

### Start Broad
Begin with the historical project memory for broad orientation, then inspect the
embedded project and design documents relevant to the subject being investigated.
Use conversation summaries afterward to understand the general shape of the
archived discussions.

A conversation summary is useful for orientation but is not an exhaustive index
of the transcript. Relevant design work may exist in a conversation even when
its summary does not mention the subject.

**Inspect a conversation summary directly**:

```bash
python3 -c 'import json; d=json.load(open("docs/prj/claude/conversations/<CONVERSATION_FILE_NAME>.json")); print(d[0]["summary"])'
```

### Search Before Reading Deeply
Search the conversation archive by subject before deciding which conversations
deserve deeper inspection. Use terminology related to the task together with
reasonable naming variants, especially when concepts or identifiers may have
changed during Chevron's development.

**Simple corpus-level discovery pass**:

```bash
rg -il '<TERM_OR_PATTERN>' docs/prj/claude/conversations/
```

**Conversation titles and summaries should not be used to determine that a topic
does or does not belong to a particular conversation. Design subjects frequently
span multiple conversations**, and later work may appear in sessions whose
stated topic suggests something else.

### Expand Context Selectively
Once relevant material has been located, inspect the matching messages and
expand outward only as necessary to understand the surrounding discussion.
**Nearby replies often provide the context needed to distinguish a proposal
from agreement, rejection, revision, or deferral**.

**Prefer targeted extraction over complete transcript reading**. Inspect larger
transcript regions only when the meaning of a relevant exchange depends on its
broader context.

When ordinary message `text` is insufficient, inspect the structured `content`
representation described in the previous section. Tool activity, generated
artifacts, attachments, and other structured material may contain context that
the rendered text does not preserve completely.

### Cross-Check the Result
Use archive findings to explain and clarify current Chevron work rather than as
a substitute for examining the repository itself. Historical reasoning is most
useful when connected back to the current documentation, implementation, or
design question that prompted the investigation.

</br>

## Chronology & Design State

**Historical design material must be interpreted in chronological context**.
The archive records Chevron's design as it evolved, so an older document or
conversation may reflect a position that was later clarified, revised, deferred,
or reversed.

### Establishing Chronology
Within a conversation, message order is chronological, and each message carries
a `created_at` timestamp. When chronology matters, use the timestamps rather
than relying only on transcript position.

**Inspect a conversation with truncated messages and timestamps**:

```bash
jq -r '.[0].chat_messages[] | [.created_at, .sender, (.text[0:120])] | @tsv' docs/prj/claude/conversations/<CONVERSATION_FILE_NAME>.json
```

Across multiple conversations, assemble chronology from the timestamps of the
relevant messages themselves. Conversation names, filenames, summaries, and
their position in the archive do not establish which discussion of a subject
came last.

Embedded project documents also represent design state at particular points in
time. **Later conversation material may clarify or supersede a document without
that change ever being propagated back into the archived document**.

Chronology establishes when evidence appeared, **not how authoritative that
evidence is**. A newer exploratory discussion does not automatically override
an older explicit architectural decision.

### Interpreting Design State
The following categories provide a useful vocabulary for interpreting evidence
found in the archive. **They are descriptive aids, not a formal decision
system**.

* **Exploration or proposal** — possibilities, candidate approaches, or ideas
  still being worked through.
* **Assistant restatement** — the assistant describing what it believes was
  established or previously agreed upon. This can provide useful supporting
  evidence, but it is secondary to direct statements from project authority.
* **Explicit architect position** — direct acceptance, rejection,
  clarification, commitment, or reaffirmation from Chevron's lead architect.
  This is strong historical evidence of intended design state.
* **Explicit deferral** — a question or decision clearly identified as
  unresolved, postponed, or intentionally left open.
* **Revision or reversal** — later material explicitly changing, narrowing, or
  overturning an earlier position.

**Do not mistake the amount of discussion, apparent consensus, or an assistant's
confident summary for proof that a design question was resolved**. Determine the
best-supported historical design state from the strongest relevant evidence
available, interpreted together with its chronology and any later contradiction
or refinement.

**When the available evidence does not clearly establish whether a design was
settled, report that uncertainty rather than converting ambiguity into a
decision**.

</br>

## Extraction & Search Techniques

Use the simplest extraction method that fits the investigation. Prefer targeted
retrieval over dumping large portions of the archive into context, and increase
the depth of inspection only when the task requires it.

**The techniques in this section are intended as reliable defaults rather than
rigid procedures**. Follow them unless the task provides a concrete reason to
adapt them. When adaptation is necessary, prefer small changes that preserve the
same goals of focused retrieval, readable output, and controlled context use.

### Embedded Project Documents
The embedded project documents inside `claude_chevron_library_project.json`
contain long lines that may be silently truncated by ordinary file-reading
tools. **Use `jq` when inspecting these documents**.

**List embedded document names**:

```bash
jq -r '.docs[].filename' docs/prj/claude/claude_chevron_library_project.json
```

**Extract the contents of a selected document**:

```bash
jq -r '.docs[] | select(.filename=="<DOCUMENT_NAME>.docx") | .content' docs/prj/claude/claude_chevron_library_project.json
```

### Conversation Discovery
**Use ripgrep for inexpensive corpus-level discovery before performing
structured message extraction**. Search using task-relevant terminology 
together with reasonable variants when names or phrasing may have changed
during Chevron's development.

**Identify conversations containing a subject**:

```bash
rg -il '<TERM_OR_PATTERN>' docs/prj/claude/conversations/
```

`rg -l`, `rg -c`, and narrowly scoped `rg -o` searches are useful for discovery
without printing entire matching lines. **Avoid broad line-dumping searches
across the conversation exports because individual JSON lines may contain very
large tool results, generated files, or other embedded content**.

**Ripgrep searches the raw conversation exports, including structured tool,
artifact, and other non-conversational content**. Treat its results as candidate
discovery rather than proof that a term appears in ordinary discussion. If
targeted message extraction produces no corresponding result, inspect the
relevant structured content.

### Targeted Message Extraction
**Use Python when extracting messages from the conversation archive**. It
provides enough flexibility to search across conversations, normalize output,
include chronology, and make small task-specific adjustments without turning
the extraction itself into a complicated procedure.

**The following provides a useful baseline for searching the full conversation
archive**:

```bash
python3 - <<'PY'
import json
from pathlib import Path
import re

pattern = re.compile(r"<TERM_OR_PATTERN>", re.IGNORECASE)

for path in Path("docs/prj/claude/conversations").glob("*.json"):
    conversation = json.loads(path.read_text())[0]

    for message in conversation["chat_messages"]:
        text = "\n".join(
            block.get("text", "")
            for block in message.get("content", [])
            if block.get("type") == "text"
        ).strip()

        if not text:
            text = message.get("text") or ""

        if pattern.search(text):
            preview = " ".join(text.split())[:300]
            print(f"\n{message['created_at']} | {path.name}"
                  f"\n{message['sender']}: {preview}")
PY
```

The output includes the message timestamp, conversation filename, sender, and a
controlled preview suitable for triage. **The baseline deliberately excludes
thinking and other structured blocks**. Search those separately when the task
requires them. **When chronology may matter, preserve the `created_at` field in
any adapted extraction**. Chronology cannot be derived reliably from conversation
names or filenames alone.

For a narrower task, adjust the pattern, preview length, selected conversations,
or surrounding context as needed. **Avoid expanding the script beyond what the
investigation actually requires**.

### Structured Content
Searching message `text` is usually sufficient for conversational design
discussion, but it is not the full-fidelity representation of every message.
**When the rendered text contains placeholders or the investigation concerns
tool activity, generated artifacts, attachments, or other structured material,
inspect the message's `content` blocks instead**.

```bash
jq -r '.[0].chat_messages[]
  | select(.created_at=="<MESSAGE_TIMESTAMP>")
  | .content[]
  | .type' \
  docs/prj/claude/conversations/<CONVERSATION_FILE_NAME>.json
```

Use the block types to determine what requires deeper inspection, then inspect
only the relevant block.

Tool inputs may contain drafted code or generated file contents, while
attachments may preserve historical documents through `extracted_content`.
**Search these deeper structures when the relevant material is not represented
completely in ordinary message text**.

Do not search every structured field by default. Begin with the cheapest useful
surface and expand into structured content only when the investigation provides
a reason to do so.

</br>

## Common Failure Modes

The following failure modes are specific to the Claude.ai archive and can cause
an investigation to miss relevant material, misread design state, or consume far
more context than necessary.

* **Trusting summaries as content indexes** — summaries are useful for
  orientation but may omit substantial portions of a conversation, including
  later design work. Summary silence does not imply transcript silence.
* **Relying only on message `text`** — structured content may be reduced to
  placeholder text in the rendered representation. Inspect `content` when tool
  activity, generated artifacts, attachments, or unsupported blocks may matter.
* **Assuming one conversation owns a topic** — design subjects frequently span
  multiple conversations. Search across the archive before limiting an
  investigation to a particular session.
* **Dumping broad raw search results** — conversation exports may contain very
  large JSON lines and embedded tool material. Prefer discovery-oriented
  searches and targeted extraction over printing complete matches.
* **Filtering user messages only as `Jamon`** — some exports identify the user as
  `human`. Cross-conversation sender filtering must account for both labels.
* **Treating archive paths as repository paths** — paths recorded inside
  conversations refer to Claude.ai sandbox environments and may not correspond
  to Chevron's current repository layout.
* **Treating an archived deferral as permanently current** — a document or
  conversation may record a question as deferred and later material may revise
  or resolve it. Interpret deferrals in chronological context.
* **Over-consuming context** — complete transcripts, tool results, and generated
  artifacts can contain large amounts of material unrelated to the task. Expand
  context only as far as the investigation requires.

**When an archive-specific failure mode is discovered repeatedly, ask to
document it here so future agents do not need to rediscover the same
limitation**.

</br>

## When to Stop

**Archive research should continue until the evidence relevant to the task has
converged, not until the archive has been exhausted**. The amount of
investigation required should **remain proportional to the importance and
uncertainty of the question being answered**.

### Signs the Investigation Is Sufficient
An investigation can usually stop when:

* the relevant historical position is supported by compatible evidence;
* the chronology is clear enough to understand how the evidence relates;
* no later contradiction, revision, or reopening of the subject has been found;
* additional searches primarily return material already examined rather than
  introducing new evidence; and
* the gathered context is sufficient to answer the actual question or support
  the work that prompted the investigation.

**Not every task requires exhaustive triangulation**. A narrow historical
question may be resolved with a small amount of strong evidence, while an
architectural or design-state investigation may require comparison across
several sources and conversations.

### Reasons to Keep Digging
Continue investigating when:

* a conversation summary omits the subject but transcript searches locate
  relevant material;
* multiple conversations present conflicting or evolving positions;
* archived documents disagree with later conversation material;
* the chronology of relevant evidence remains unclear;
* it is uncertain whether a position was explored, accepted, deferred, revised,
  or resolved; or
* the conclusion will materially affect implementation, architecture, or an
  open design question and the available evidence remains ambiguous.

When reasonable investigation does not produce convergence, **report the
remaining uncertainty rather than continuing indefinitely or manufacturing a
definitive historical answer**.
