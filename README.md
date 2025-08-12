# MyWallet

A SvelteKit + TypeScript frontend paired with a C++ backend that manages wallets, transactions, and transfers stored in a local SQLite database.

This document explains:
- How the project is structured
- How the main parts interact and the flow of instructions
- How the frontend talks to the backend
- How the backend components interact
- What each file in include_cpp does
- What each file in the project does (excluding node_modules, build output, and dot-directories)

---

## 1) High-level Architecture

- Frontend (SvelteKit, TypeScript, TailwindCSS)
  - Provides UI to view, create, update, and delete wallets and records (transactions/transfers).
  - Renders pages via Svelte components under src/routes.
- Backend (C++, SQLite via sqlite3)
  - Encapsulates data access logic and domain operations in a WalletManager component.
  - Manages tables, CRUD operations, and transactional logic in a local SQLite database.
- Database (SQLite)
  - Stores wallets and their records. Tables are initialized at startup by the backend.

Conceptual interaction:
1. User interacts with the UI.
2. The UI triggers an action (e.g., “Create wallet”, “Record transaction”).
3. The frontend calls the backend operation (HTTP API, CLI invocation, or other IPC depending on deployment).
4. The backend uses WalletManager to perform the DB operation.
5. The backend returns a result (JSON string or status), and UI updates accordingly.

Note: This repository contains both the SvelteKit app and the C++ sources. The exact runtime wiring (e.g., invoking a local service or exposing HTTP endpoints) can be configured per deployment. The backend currently encapsulates all data manipulation operations and returns results as strings (commonly JSON-encoded), which the frontend can consume.

---

## 2) Flow of Instructions

- App initialization:
  - Frontend: SvelteKit loads src/routes/+layout.svelte and renders src/routes/+page.svelte.
  - Backend: The C++ executable initializes/opens the SQLite database and ensures required tables exist.
- User actions:
  - UI events dispatch requests to backend operations (e.g., “create_wallet”, “get_wallets”, “record_transaction”, etc.).
  - The backend’s WalletManager:
    - Opens the database connection if needed.
    - Executes a prepared statement against SQLite.
    - Maps results to a string payload (typically JSON).
- Data update:
  - On success, the backend returns the resulting data or confirmation.
  - Frontend updates its component state and re-renders with the new data.

---

## 3) Frontend ↔ Backend Interactions

- Request types:
  - Query operations: get_wallets, get_records
  - Mutation operations: create_wallet, update_wallet, delete_wallet
  - Record operations: record_transaction, record_transfer, update_record, update_transfer, delete_record
- Payloads:
  - Typically strings that encode criteria or objects to create/update (e.g., JSON text).
  - Backend returns strings (commonly JSON) with results or error messages.
- Transport:
  - The repository does not force a single transport; you may:
    - Run the C++ backend as a local process and invoke commands/arguments.
    - Wrap the backend in a small HTTP service.
    - Bind it via native addon/FFI in a Node server route.
  - Choose the integration strategy that best fits your deployment model.

---

## 4) Backend components and their interactions

- WalletManager (include_cpp/walletManager.h + src_cpp/*)
  - Central API for all wallet/record operations.
  - Responsibilities:
    - Initialize and close the SQLite database.
    - Create and maintain tables.
    - Provide CRUD methods for wallets and records.
    - Use prepared statements for safe DB access.
- Data structures
  - wallet (include_cpp/wallet.h)
    - Represents a wallet entity (e.g., name, id, balance or related metadata).
  - transaction (include_cpp/transaction.h)
    - Represents a record/transaction, including wallet_name and related fields.
- DB bootstrap
  - init_db and init_tables set up SQLite database and tables (wallets and related record tables).
- Operation files (src_cpp)
  - Provide method definitions corresponding to WalletManager’s API:
    - get_wallets, create_wallet, update_wallet, delete_wallet
    - get_records, record_transaction, update_record, delete_record
    - record_transfer, update_transfer

Lifecycle:
- On first run, init_db creates the database directory/file and initializes tables.
- Subsequent operations open the DB, execute statements, and return results.
- All DB handles are closed properly to avoid leaks.

---

## 5) Per-file documentation (project root)

- README.md
  - This documentation.
- LICENSE
  - License for the project.
- package.json
  - Frontend package metadata, scripts, and dependencies (pnpm recommended).
- pnpm-lock.yaml
  - Locked dependency versions for reproducible installs.
- vite.config.ts
  - Vite configuration tailored for SvelteKit and development tooling.
- svelte.config.js
  - SvelteKit configuration (adapters, preprocessors).
- tailwind.config.ts
  - TailwindCSS configuration, including paths to purge and theme customizations.
- postcss.config.js
  - PostCSS pipeline configuration (e.g., autoprefixer).
- eslint.config.js
  - ESLint configuration using @eslint/js, eslint-plugin-svelte, typescript-eslint, etc.
- tsconfig.json
  - TypeScript configuration for the frontend codebase.
- .prettierrc / .prettierignore
  - Prettier formatting rules and ignore patterns.
- .npmrc
  - npm/pnpm configuration (e.g., registry, auto-fixups).
- .gitignore
  - Git ignored files and folders.
- CMakeLists.cpp
  - CMake configuration for building the C++ backend executable/library (project name, sources, compiler flags).
- main.cpp
  - Backend entry point. Sets up the program (e.g., identifies the application as “myWallet”), invokes database initialization, and can dispatch to WalletManager operations.

Excluded from this documentation: node_modules, build output folders, and “dot” directories as requested.

---

## 6) Per-file documentation (frontend: src and static)

- src/app.html
  - Top-level HTML template for SvelteKit pages (document head, title, body).
- src/app.css
  - Global CSS (Tailwind’s base, components, utilities can be included here).
- src/app.d.ts
  - Ambient TypeScript declarations for the SvelteKit app.
- src/lib/
  - Reusable frontend modules/assets.
  - src/lib/assets/favicon.svg: App favicon.
  - src/lib/index.ts: Barrel for shared exports (utility functions, types, or stores).
- src/routes/
  - +layout.svelte: Root layout component applied to all pages (global UI scaffolding).
  - +page.svelte: The main page component (renders the wallet UI, triggers data loading and actions).
- static/
  - Publicly served static assets (copied to the final build as-is).

---

## 7) Per-file documentation (backend headers: include_cpp)

- include_cpp/my.h
  - Aggregator header that includes walletManager.h to expose the backend API in one place.
- include_cpp/wallet.h
  - Defines the wallet data structure (fields for identifying a wallet and related metadata).
- include_cpp/transaction.h
  - Defines the transaction/record data structure (e.g., wallet_name, amounts, timestamps, types).
- include_cpp/walletManager.h
  - Declares the WalletManager class:
    - Database lifecycle: init_db, init_tables, closedb
    - Retrieval: get_wallets, get_records
    - Wallet CRUD: create_wallet, update_wallet, delete_wallet
    - Record ops: record_transaction, update_record, delete_record
    - Transfer ops: record_transfer, update_transfer

---

## 8) Per-file documentation (backend sources: src_cpp)

- src_cpp/init_db.cpp
  - Implements database initialization:
    - Computes DB path/location (e.g., a “myWallet.sqlite” file under a “myWallet” directory).
    - Creates tables (wallets and related record tables) if not present.
    - Provides closedb for safely closing sqlite3 connections.
- src_cpp/get_wallets.cpp
  - Implements WalletManager::get_wallets to retrieve wallets (optionally filtered by criteria).
- src_cpp/create_wallet.cpp
  - Implements WalletManager::create_wallet to insert a new wallet.
- src_cpp/update_wallet.cpp
  - Implements WalletManager::update_wallet to modify wallet details.
- src_cpp/delete_wallet.cpp
  - Implements WalletManager::delete_wallet to remove a wallet (and possibly cascade/validate).
- src_cpp/get_records.cpp
  - Implements WalletManager::get_records to list transactions/records (optionally filtered).
- src_cpp/record_transaction.cpp
  - Implements WalletManager::record_transaction to add a new transaction/record.
- src_cpp/update_record.cpp
  - Implements WalletManager::update_record and includes prepared statement helper(s).
- src_cpp/delete_record.cpp
  - Implements WalletManager::delete_record to remove a record.
- src_cpp/record_transfer.cpp
  - Implements WalletManager::record_transfer to add a new transfer between wallets.
- src_cpp/update_transfer.cpp
  - Implements WalletManager::update_transfer to modify an existing transfer.

---

## 9) Development and Build

Frontend:
- Install dependencies
  - pnpm install
- Start dev server
  - pnpm run dev
  - Optionally open browser: pnpm dev -- --open
- Build production
  - pnpm run build

Backend (C++ with CMake):
- Configure and build (example workflow)
  - Create a build directory.
    - Inside the build directory, run 
      ```
      cmake .. && cmake --build .                                       (On linux and MacOs)
      cmake .. ; cmake --build .                                        (On Windows)
