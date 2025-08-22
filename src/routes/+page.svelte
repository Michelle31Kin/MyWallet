<!-- src/routes/+page.svelte
A SIMPLE, READABLE, TAILWIND-ONLY single-file SvelteKit page for your Wallet Tracker.

Layout:
- Navbar (top) with backend status + actions (Init DB, Reload)
- Left panel (Home / Archived). Under "Archived", each soft-deleted wallet shows a RESTORE button IN FRONT of the wallet name
- Center panel: a small "dock" (quick focus & totals), Wallet Cards grid, and Transactions table (all wallets by default; filtered by focused wallet)
- Right mini retractable panel with simple actions: Create Wallet, New Income/Expense/Transfer, Edit Focused Wallet

Backend assumptions (from your uploaded C++ files):
window.saucer.exposed has:
  init_db()
  get_wallets()
  create_wallet(json)
  update_wallet(json)
  delete_wallet(json)                  // soft delete
  delete_wallet_permanently(json)      // hard delete
  restore_wallets(json)                // restore by name or all
  get_records()
  record_transaction(json)             // wallet_name, type: INCOME|EXPENSE, amount, category?, description?
  record_transfer(json)                // from_wallet_name, to_wallet_name, amount, category?, description?
(We only call update_record/update_transfer/delete_record if you add buttons later.)

Key rule you requested:
- If operation is INCOME or EXPENSE, "related_wallet" should be treated as the SAME wallet. We enforce this logically in the UI and do not send an extra field.

Make sure Tailwind is enabled in your SvelteKit project.
-->

<script lang="ts">
  import { onMount } from 'svelte';

  type Wallet = {
    rowid?: number;
    name: string;
    currency: string;
    source: string;
    initial_amount: number;
    balance: number;
    color?: string | null;
    created_at?: string | null;
    updated_at?: string | null;
    is_active?: number; // 1 active, 0 soft-deleted
  };

  type RecordEntry = {
    id: number;
    wallet_name: string;
    related_wallet_name?: string | null;
    type: 'INCOME' | 'EXPENSE' | 'transfer_in' | 'transfer_out';
    amount: number;
    category?: string | null;
    description?: string | null;
    created_at?: string | null;
    updated_at?: string | null;
    transfer_id?: number | null;
  };

  // ----------------------- Backend wiring -----------------------
  function getApi(): any {
    return (window as any)?.saucer?.exposed;
  }
  async function call<T=any>(fn: string, payload?: any): Promise<T> {
    const api = getApi();
    if (!api || typeof api[fn] !== 'function') throw new Error(`Backend method missing: ${fn}`);
    const raw = payload === undefined ? await api[fn]() : await api[fn](JSON.stringify(payload));
    if (typeof raw === 'string') {
      try { return JSON.parse(raw) as T; } catch { /* fallthrough */ }
    }
    return raw as T;
  }

  // ----------------------- State -----------------------
  let backendOK = false;
  let loading = true;
  let view: 'home' | 'archived' = 'home';
  let showRight = true;

  let wallets: Wallet[] = [];
  let records: RecordEntry[] = [];
  let focusedWallet: string | null = null;

  // Create wallet
  let newWallet = {
    name: '',
    currency: 'USD',
    source: 'manual',
    initial_amount: 0,
    balance: 0,
    color: '#2563eb'
  };

  // Quick transaction
  let opType: 'INCOME' | 'EXPENSE' | 'TRANSFER' = 'INCOME';
  let tx = {
    wallet_name: '',
    to_wallet_name: '',
    amount: 0,
    category: 'General',
    description: ''
  };

  // Edit focused wallet (simple)
  let editWallet = {
    name: '',
    currency: 'USD',
    source: 'manual',
    balance: 0,
    color: '#2563eb'
  };

  // Alerts
  type Alert = { kind: 'success' | 'error' | 'info', text: string, id: number };
  let alerts: Alert[] = [];
  let nextAlertId = 1;
  function notify(kind: Alert['kind'], text: string) {
    const id = nextAlertId++;
    alerts = [...alerts, { kind, text, id }];
    setTimeout(() => { alerts = alerts.filter(a => a.id !== id); }, 3500);
  }

  // Helpers
  function fmtMoney(v: number, cur='USD') {
    if (Number.isNaN(v)) return '—';
    try { return new Intl.NumberFormat(undefined, { style:'currency', currency:cur }).format(v); }
    catch { return `${v.toFixed(2)} ${cur}`; }
  }
  function fmtDate(s?: string | null) {
    if (!s) return '—';
    const d = new Date(s);
    return isNaN(d.getTime()) ? s : d.toLocaleString();
  }

  // Derived
  $: activeWallets = wallets.filter(w => (w.is_active ?? 1) === 1);
  $: archivedWallets = wallets.filter(w => (w.is_active ?? 1) === 0);
  $: filteredRecords = focusedWallet
    ? records.filter(r => r.wallet_name === focusedWallet || r.related_wallet_name === focusedWallet)
    : records;

  // Totals (by currency)
  $: totals = activeWallets.reduce(
    (acc, w) => {
      acc.byCurrency[w.currency] = (acc.byCurrency[w.currency] || 0) + (w.balance || 0);
      return acc;
    },
    { byCurrency: {} as Record<string, number> }
  );

  // ----------------------- Load -----------------------
  async function reloadAll() {
    loading = true;
    try {
      const w = await call<Wallet[]>('get_wallets');
      backendOK = true;
      wallets = (w || []).map((x: any) => ({
        rowid: x.rowid ?? x.id ?? undefined,
        name: x.name,
        currency: x.currency ?? 'USD',
        source: x.source ?? 'manual',
        initial_amount: Number(x.initial_amount ?? 0),
        balance: Number(x.balance ?? 0),
        color: x.color ?? '#2563eb',
        created_at: x.created_at ?? null,
        updated_at: x.updated_at ?? null,
        is_active: x.is_active ?? 1
      }));
      const r = await call<RecordEntry[]>('get_records');
      records = (r || []).map((x:any) => ({
        id: Number(x.id),
        wallet_name: x.wallet_name,
        related_wallet_name: x.related_wallet_name ?? null,
        type: x.type,
        amount: Number(x.amount ?? 0),
        category: x.category ?? null,
        description: x.description ?? null,
        created_at: x.created_at ?? null,
        updated_at: x.updated_at ?? null,
        transfer_id: x.transfer_id ?? null
      }));
      if (wallets.length) {
        // set sane defaults for forms
        if (!tx.wallet_name) tx.wallet_name = wallets[0].name;
        tx.to_wallet_name = wallets.length > 1 ? wallets[1].name : wallets[0].name;
        const w0 = wallets[0];
        editWallet = { name: w0.name, currency: w0.currency, source: w0.source, balance: w0.balance, color: w0.color || '#2563eb' };
      }
    } catch (e:any) {
      backendOK = false;
      notify('error', e.message || 'Backend connection failed');
    } finally {
      loading = false;
    }
  }

  onMount(reloadAll);

  // ----------------------- Actions -----------------------
  async function doInitDb() {
    try { await call('init_db'); notify('success', 'Database initialized'); await reloadAll(); }
    catch (e:any) { notify('error', e.message || 'init_db failed'); }
  }

  async function createWallet() {
    if (!newWallet.name.trim()) return notify('error', 'Wallet name is required');
    try {
      await call('create_wallet', newWallet);
      notify('success', `Wallet "${newWallet.name}" created`);
      newWallet = { name:'', currency:'USD', source:'manual', initial_amount:0, balance:0, color:'#2563eb' };
      await reloadAll();
    } catch (e:any) { notify('error', e.message || 'Create wallet failed'); }
  }

  async function updateFocusedWallet() {
    if (!focusedWallet) return notify('error', 'Focus a wallet first');
    try {
      await call('update_wallet', editWallet);
      notify('success', `Wallet "${editWallet.name}" updated`);
      await reloadAll();
    } catch (e:any) { notify('error', e.message || 'Update failed'); }
  }

  async function softDelete(name: string) {
    if (!confirm(`Archive wallet "${name}"?`)) return;
    try { await call('delete_wallet', { name }); notify('success', `Wallet "${name}" archived`); if (focusedWallet===name) focusedWallet=null; await reloadAll(); }
    catch (e:any) { notify('error', e.message || 'Archive failed'); }
  }

  async function hardDelete(name: string) {
    if (!confirm(`PERMANENTLY delete "${name}"? This cannot be undone.`)) return;
    try { await call('delete_wallet_permanently', { name }); notify('success', `Wallet "${name}" deleted`); if (focusedWallet===name) focusedWallet=null; await reloadAll(); }
    catch (e:any) { notify('error', e.message || 'Delete failed'); }
  }

  async function restore(name: string) {
    try { await call('restore_wallets', { name }); notify('success', `Wallet "${name}" restored`); await reloadAll(); }
    catch (e:any) { notify('error', e.message || 'Restore failed'); }
  }

  function setFocus(name: string) {
    focusedWallet = focusedWallet === name ? null : name;
    const w = wallets.find(x => x.name === (focusedWallet || ''));
    if (w) {
      editWallet = { name: w.name, currency: w.currency, source: w.source, balance: w.balance, color: w.color || '#2563eb' };
      tx.wallet_name = w.name;
    }
  }

  // Income/Expense: related_wallet is the SAME as the wallet (logic enforced, field not required for backend call)
  async function saveIncomeExpense() {
    if (!tx.wallet_name) return notify('error', 'Choose a wallet');
    if (opType === 'TRANSFER') return notify('error', 'Switch type to INCOME or EXPENSE');
    if (Number(tx.amount) <= 0) return notify('error', 'Enter an amount > 0');

    try {
      await call('record_transaction', {
        wallet_name: tx.wallet_name,
        type: opType,
        amount: Number(tx.amount),
        category: tx.category,
        description: tx.description
      });
      notify('success', `${opType} recorded on ${tx.wallet_name}`);
      tx.amount = 0; tx.description = '';
      await reloadAll();
    } catch (e:any) { notify('error', e.message || `Failed to record ${opType}`); }
  }

  async function saveTransfer() {
    if (opType !== 'TRANSFER') return notify('error', 'Switch type to TRANSFER');
    if (!tx.wallet_name || !tx.to_wallet_name) return notify('error', 'Choose both wallets');
    if (tx.wallet_name === tx.to_wallet_name) return notify('error', 'Choose two different wallets');
    if (Number(tx.amount) <= 0) return notify('error', 'Enter an amount > 0');

    try {
      await call('record_transfer', {
        from_wallet_name: tx.wallet_name,
        to_wallet_name: tx.to_wallet_name,
        amount: Number(tx.amount),
        category: tx.category,
        description: tx.description
      });
      notify('success', `Transfer recorded: ${tx.wallet_name} → ${tx.to_wallet_name}`);
      tx.amount = 0; tx.description = '';
      await reloadAll();
    } catch (e:any) { notify('error', e.message || 'Transfer failed'); }
  }
</script>

<!-- ============================ NAVBAR ============================ -->
<header class="sticky top-0 z-20 bg-slate-900/80 backdrop-blur border-b border-slate-800">
  <div class="max-w-7xl mx-auto h-14 px-4 flex items-center justify-between">
    <div class="flex items-center gap-3">
      <div class="h-8 w-8 rounded bg-sky-600 grid place-items-center text-white font-bold">W</div>
      <div class="text-lg font-semibold text-slate-100">Wallet Tracker</div>
      <div class="text-sm ml-3">
        {#if backendOK}
          <span class="px-2 py-0.5 rounded bg-emerald-700 text-emerald-50">Backend: Connected</span>
        {:else}
          <span class="px-2 py-0.5 rounded bg-rose-700 text-rose-50">Backend: Not connected</span>
        {/if}
      </div>
    </div>
    <div class="flex items-center gap-2">
      <button class="px-3 py-1.5 rounded-md bg-slate-800 text-slate-100 text-sm hover:bg-slate-700" on:click={doInitDb}>
        Init DB
      </button>
      <button class="px-3 py-1.5 rounded-md bg-slate-800 text-slate-100 text-sm hover:bg-slate-700" on:click={reloadAll}>
        Reload
      </button>
      <button class="px-3 py-1.5 rounded-md bg-slate-800 text-slate-100 text-sm hover:bg-slate-700" on:click={() => showRight = !showRight}>
        {showRight ? 'Hide' : 'Show'} Panel
      </button>
    </div>
  </div>
</header>

<!-- ============================ PAGE GRID ============================ -->
<div class="max-w-7xl mx-auto px-4 py-4 grid grid-cols-12 gap-4">
  <!-- LEFT PANEL -->
  <aside class="col-span-12 md:col-span-2">
    <div class="rounded-lg border border-slate-800 bg-slate-900 p-3">
      <div class="text-xs uppercase tracking-wide text-slate-400 mb-2">Options</div>
      <nav class="space-y-1">
        <button
          class="w-full text-left px-3 py-2 rounded-md text-sm transition
                 border border-slate-800 hover:border-slate-700
                 {view==='home' ? 'bg-slate-800 text-slate-100' : 'bg-slate-900 text-slate-300'}"
          on:click={() => (view = 'home')}>
          Home
        </button>
        <button
          class="w-full text-left px-3 py-2 rounded-md text-sm transition
                 border border-slate-800 hover:border-slate-700
                 {view==='archived' ? 'bg-slate-800 text-slate-100' : 'bg-slate-900 text-slate-300'}"
          on:click={() => (view = 'archived')}>
          Archived
        </button>
      </nav>

      {#if view === 'archived'}
        <div class="mt-4">
          <div class="text-xs uppercase tracking-wide text-slate-400 mb-2">Soft-deleted</div>
          {#if archivedWallets.length === 0}
            <div class="text-slate-400 text-sm">None</div>
          {:else}
            <ul class="space-y-2">
              {#each archivedWallets as w}
                <li class="flex items-center justify-between gap-2">
                  <!-- Restore button IN FRONT of each deleted wallet -->
                  <button class="px-2 py-1 rounded-md bg-emerald-600 text-white text-xs hover:bg-emerald-500"
                    on:click={() => restore(w.name)}>
                    Restore
                  </button>
                  <span class="flex-1 truncate text-slate-200 text-sm">{w.name}</span>
                  <button class="px-2 py-1 rounded-md bg-rose-700 text-white text-xs hover:bg-rose-600"
                    on:click={() => hardDelete(w.name)}>
                    Delete
                  </button>
                </li>
              {/each}
            </ul>
          {/if}
        </div>
      {/if}
    </div>
  </aside>

  <!-- CENTER PANEL -->
  <main class="col-span-12 md:col-span-8 space-y-4">
    <!-- Dock -->
    <section class="rounded-lg border border-slate-800 bg-slate-900 p-3">
      <div class="flex flex-col sm:flex-row sm:items-center sm:justify-between gap-2">
        <div class="text-base text-slate-200">
          {#if focusedWallet}
            Focus: <span class="font-semibold text-sky-400">{focusedWallet}</span>
          {:else}
            Focus: <span class="font-semibold text-sky-400">All wallets</span>
          {/if}
        </div>
        <div class="flex items-center gap-2 text-sm">
          <button class="px-3 py-1.5 rounded-md bg-slate-800 text-slate-100 hover:bg-slate-700" on:click={() => (focusedWallet = null)}>
            Clear Focus
          </button>
          <div class="hidden sm:flex items-center gap-2">
            {#each Object.entries(totals.byCurrency) as [cur, amt]}
              <span class="px-2 py-1 rounded bg-slate-800 text-slate-100">{cur}: {fmtMoney(amt as number, cur)}</span>
            {/each}
            {#if Object.keys(totals.byCurrency).length === 0}
              <span class="px-2 py-1 rounded bg-slate-800 text-slate-400">Totals: —</span>
            {/if}
          </div>
        </div>
      </div>
    </section>

    <!-- Wallet Cards -->
    <section class="rounded-lg border border-slate-800 bg-slate-900 p-3">
      <div class="flex items-center justify-between mb-2">
        <h2 class="text-slate-100 font-semibold text-lg">Wallets</h2>
        <div class="text-sm text-slate-400">{activeWallets.length} active</div>
      </div>
      {#if loading}
        <div class="p-4 text-slate-400 text-base">Loading wallets…</div>
      {:else if activeWallets.length === 0}
        <div class="p-4 text-slate-400 text-base">No wallets. Use the right panel to create one.</div>
      {:else}
        <div class="grid grid-cols-1 sm:grid-cols-2 lg:grid-cols-3 gap-3">
          {#each activeWallets as w (w.name)}
            <article
              class="rounded-lg border border-slate-700 bg-slate-800 p-3 hover:ring-2 hover:ring-sky-600 transition cursor-pointer"
              on:click={() => setFocus(w.name)}>
              <div class="flex items-center justify-between">
                <div class="flex items-center gap-2">
                  <div class="h-3 w-3 rounded-full ring-2 ring-slate-900" style="background: {w.color || '#2563eb'}"></div>
                  <h3 class="text-slate-100 font-medium truncate text-base">{w.name}</h3>
                </div>
                <span class="text-xs text-slate-300">{w.currency}</span>
              </div>
              <div class="mt-2 text-slate-100 text-xl font-semibold">{fmtMoney(w.balance, w.currency)}</div>
              <div class="mt-3 flex items-center gap-2">
                <button class="px-2 py-1 rounded-md bg-slate-700 text-slate-100 text-xs hover:bg-slate-600"
                        on:click|stopPropagation={() => setFocus(w.name)}>
                  Focus
                </button>
                <button class="px-2 py-1 rounded-md bg-amber-600 text-white text-xs hover:bg-amber-500"
                        on:click|stopPropagation={() => softDelete(w.name)}>
                  Archive
                </button>
                <button class="px-2 py-1 rounded-md bg-rose-700 text-white text-xs hover:bg-rose-600"
                        on:click|stopPropagation={() => hardDelete(w.name)}>
                  Delete
                </button>
              </div>
              <div class="mt-2 text-xs text-slate-400">
                <div>Created: {fmtDate(w.created_at)}</div>
                <div>Updated: {fmtDate(w.updated_at)}</div>
              </div>
            </article>
          {/each}
        </div>
      {/if}
    </section>

    <!-- Transactions -->
    <section class="rounded-lg border border-slate-800 bg-slate-900 p-3">
      <div class="flex items-center justify-between mb-2">
        <h2 class="text-slate-100 font-semibold text-lg">Transactions</h2>
        <div class="text-sm text-slate-400">{filteredRecords.length} items</div>
      </div>
      {#if loading}
        <div class="p-4 text-slate-400 text-base">Loading transactions…</div>
      {:else if filteredRecords.length === 0}
        <div class="p-4 text-slate-400 text-base">No transactions.</div>
      {:else}
        <div class="overflow-x-auto">
          <table class="min-w-full text-sm">
            <thead class="text-slate-200 bg-slate-800">
              <tr class="border-b border-slate-700">
                <th class="text-left py-2 px-3">#</th>
                <th class="text-left py-2 px-3">Wallet</th>
                <th class="text-left py-2 px-3">Related</th>
                <th class="text-left py-2 px-3">Type</th>
                <th class="text-right py-2 px-3">Amount</th>
                <th class="text-left py-2 px-3">Category</th>
                <th class="text-left py-2 px-3">Note</th>
                <th class="text-left py-2 px-3">When</th>
              </tr>
            </thead>
            <tbody class="text-slate-100">
              {#each filteredRecords as r}
                <tr class="border-b border-slate-800">
                  <td class="py-2 px-3">#{r.id}</td>
                  <td class="py-2 px-3">{r.wallet_name}</td>
                  <td class="py-2 px-3">{r.related_wallet_name || '—'}</td>
                  <td class="py-2 px-3">
                    <span class="px-2 py-0.5 rounded-full text-xs
                      {r.type==='INCOME' ? 'bg-emerald-700' : r.type==='EXPENSE' ? 'bg-amber-700' : 'bg-sky-700'}">
                      {r.type}
                    </span>
                  </td>
                  <td class="py-2 px-3 text-right">{fmtMoney(r.amount, wallets.find(w=>w.name===r.wallet_name)?.currency || 'USD')}</td>
                  <td class="py-2 px-3">{r.category || '—'}</td>
                  <td class="py-2 px-3 truncate max-w-[30ch]" title={r.description || ''}>{r.description || '—'}</td>
                  <td class="py-2 px-3">{fmtDate(r.created_at)}</td>
                </tr>
              {/each}
            </tbody>
          </table>
        </div>
      {/if}
    </section>
  </main>

  <!-- RIGHT MINI PANEL -->
  <aside class="col-span-12 md:col-span-2">
    <div class="relative">
      <div class={`transition-all duration-200 ${showRight ? 'translate-x-0 opacity-100' : 'translate-x-3 opacity-0 pointer-events-none'}`}>
        <div class="rounded-lg border border-slate-800 bg-slate-900 p-3 space-y-4">
          <div class="text-xs uppercase tracking-wide text-slate-400">Quick Actions</div>

          <!-- Create Wallet -->
          <div class="space-y-2">
            <div class="text-slate-100 font-medium">Create Wallet</div>
            <input class="w-full px-3 py-2 rounded-md bg-slate-800 border border-slate-700 text-sm" placeholder="Name" bind:value={newWallet.name} />
            <div class="grid grid-cols-2 gap-2">
              <input class="px-3 py-2 rounded-md bg-slate-800 border border-slate-700 text-sm" placeholder="Currency" bind:value={newWallet.currency} />
              <input type="color" class="px-3 py-2 rounded-md bg-slate-800 border border-slate-700 text-sm h-[42px]" bind:value={newWallet.color} />
            </div>
            <div class="grid grid-cols-2 gap-2">
              <input type="number" class="px-3 py-2 rounded-md bg-slate-800 border border-slate-700 text-sm" placeholder="Initial" bind:value={newWallet.initial_amount} />
              <input type="number" class="px-3 py-2 rounded-md bg-slate-800 border border-slate-700 text-sm" placeholder="Balance" bind:value={newWallet.balance} />
            </div>
            <input class="w-full px-3 py-2 rounded-md bg-slate-800 border border-slate-700 text-sm" placeholder="Source" bind:value={newWallet.source} />
            <button class="w-full px-3 py-2 rounded-md bg-emerald-600 text-white text-sm hover:bg-emerald-500" on:click={createWallet}>
              Create
            </button>
          </div>

          <!-- New Transaction -->
          <div class="space-y-2">
            <div class="text-slate-100 font-medium">New Transaction</div>
            <div class="grid grid-cols-2 gap-2">
              <select class="px-3 py-2 rounded-md bg-slate-800 border border-slate-700 text-sm" bind:value={opType}>
                <option value="INCOME">Income</option>
                <option value="EXPENSE">Expense</option>
                <option value="TRANSFER">Transfer</option>
              </select>
              <select class="px-3 py-2 rounded-md bg-slate-800 border border-slate-700 text-sm" bind:value={tx.wallet_name}>
                <option value="" disabled selected hidden>Wallet</option>
                {#each activeWallets as w}
                  <option value={w.name}>{w.name}</option>
                {/each}
              </select>
            </div>

            {#if opType === 'TRANSFER'}
              <select class="w-full px-3 py-2 rounded-md bg-slate-800 border border-slate-700 text-sm" bind:value={tx.to_wallet_name}>
                <option value="" disabled selected hidden>To wallet</option>
                {#each activeWallets as w}
                  <option value={w.name}>{w.name}</option>
                {/each}
              </select>
            {/if}

            <input type="number" min="0" step="0.01" class="w-full px-3 py-2 rounded-md bg-slate-800 border border-slate-700 text-sm" placeholder="Amount" bind:value={tx.amount} />
            <input class="w-full px-3 py-2 rounded-md bg-slate-800 border border-slate-700 text-sm" placeholder="Category" bind:value={tx.category} />
            <input class="w-full px-3 py-2 rounded-md bg-slate-800 border border-slate-700 text-sm" placeholder="Description" bind:value={tx.description} />

            {#if opType === 'TRANSFER'}
              <button class="w-full px-3 py-2 rounded-md bg-sky-600 text-white text-sm hover:bg-sky-500" on:click={saveTransfer}>
                Save Transfer
              </button>
            {:else}
              <button class="w-full px-3 py-2 rounded-md bg-sky-600 text-white text-sm hover:bg-sky-500" on:click={saveIncomeExpense}>
                Save {opType}
              </button>
            {/if}
          </div>

          <!-- Edit Focused Wallet -->
          <div class="space-y-2">
            <div class="text-slate-100 font-medium">Edit Focused Wallet</div>
            {#if focusedWallet}
              <div class="text-xs text-slate-400">Editing: <span class="text-slate-200">{focusedWallet}</span></div>
              <input class="w-full px-3 py-2 rounded-md bg-slate-800 border border-slate-700 text-sm" placeholder="Name (PK)" bind:value={editWallet.name} />
              <div class="grid grid-cols-2 gap-2">
                <input class="px-3 py-2 rounded-md bg-slate-800 border border-slate-700 text-sm" placeholder="Currency" bind:value={editWallet.currency} />
                <input type="color" class="px-3 py-2 rounded-md bg-slate-800 border border-slate-700 text-sm h-[42px]" bind:value={editWallet.color} />
              </div>
              <div class="grid grid-cols-2 gap-2">
                <input class="px-3 py-2 rounded-md bg-slate-800 border border-slate-700 text-sm" placeholder="Source" bind:value={editWallet.source} />
                <input type="number" class="px-3 py-2 rounded-md bg-slate-800 border border-slate-700 text-sm" placeholder="Balance" bind:value={editWallet.balance} />
              </div>
              <button class="w-full px-3 py-2 rounded-md bg-indigo-600 text-white text-sm hover:bg-indigo-500" on:click={updateFocusedWallet}>
                Update Wallet
              </button>
            {:else}
              <div class="text-slate-400 text-sm">Focus a wallet from the center to edit it here.</div>
            {/if}
          </div>
        </div>
      </div>

      <!-- Handle -->
      <button
        class="absolute -left-3 top-1/2 -translate-y-1/2 px-2 py-1 rounded-md bg-slate-800 border border-slate-700 text-slate-200 text-xs"
        on:click={() => (showRight = !showRight)}>
        {showRight ? '⟨' : '⟩'}
      </button>
    </div>
  </aside>
</div>

<!-- ALERTS -->
<div class="fixed bottom-4 right-4 space-y-2 z-30">
  {#each alerts as a (a.id)}
    <div class="px-3 py-2 rounded-md border text-sm shadow
                {a.kind==='success' ? 'bg-emerald-900/90 border-emerald-700 text-emerald-50' :
                 a.kind==='error'   ? 'bg-rose-900/90 border-rose-700 text-rose-50' :
                                      'bg-slate-800/90 border-slate-700 text-slate-50'}">
      {a.text}
    </div>
  {/each}
</div>

<style>
  :global(html, body) { @apply bg-slate-950 text-slate-100; }
</style>
