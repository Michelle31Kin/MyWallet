<script lang="ts">
	// MyWallet – Function Tester (Svelte 5 / runes)
	// Transport "window" calls window.saucer.exposed.<fn>
	// Transport "http" calls /api/<fn>
	// Transport "auto" tries window first, then HTTP

	type Kind = 'query' | 'mutation';

	type Operation = {
		key: keyof Window['saucer']['exposed'];
		label: string;
		kind: Kind;
		help: string;
		sample: any;
	};

	const sampleWallet = {
		name: "Cash",
		currency: "USD",
		source: "manual",
		initial_amount: 0,
		balance: 0,
		color: "#7c3aed",
		created_at: "2025-08-14 12:00:00",
		updated_at: "2025-08-14 12:00:00"
	};

	const sampleTransaction = {
		id: 1,
		wallet_name: "Cash",
		type: "INCOME", // INCOME | EXPENSE | TRANSFER
		recorded_at: "2025-08-14 12:00:00",
		updated_at: "2025-08-14 12:00:00",
		category: "General",
		description: "Test note",
		amount: 100,
		related_wallet_name: "Bank" // used for transfers
	};

	const operations: Operation[] = [
		// Wallets
		{ key: "create_wallet", label: "Create Wallet", kind: "mutation", help: "Insert into wallets table.", sample: { ...sampleWallet } },
		{ key: "update_wallet", label: "Update Wallet", kind: "mutation", help: "Update wallet fields by name.", sample: { ...sampleWallet, color: "#2563eb" } },
		{ key: "delete_wallet", label: "Delete Wallet (soft)", kind: "mutation", help: "Mark wallet inactive and archive transactions.", sample: { name: "Cash" } },
		{ key: "delete_wallet_permanently", label: "Delete Wallet (permanent)", kind: "mutation", help: "Permanently mark wallet inactive (same as soft in current code).", sample: { name: "Cash" } },
		{ key: "get_wallets", label: "Get Wallets", kind: "query", help: "Return wallets list. Criteria currently ignored.", sample: {} },

		// Records
		{ key: "record_transaction", label: "Record Transaction", kind: "mutation", help: "Insert a transaction and adjust wallet balance.", sample: { ...sampleTransaction, type: "INCOME", amount: 150 } },
		{ key: "update_record", label: "Update Record", kind: "mutation", help: "Update transaction and adjust balances accordingly.", sample: { id: 1, wallet_name: "Cash", type: "EXPENSE", amount: 40, description: "Corrected", updated_at: "2025-08-14 12:05:00" } },
		{ key: "delete_record", label: "Delete Record", kind: "mutation", help: "Delete transaction by id and reverse balance effect.", sample: { id: 1, wallet_name: "Cash", type: "INCOME", amount: 150 } },
		{ key: "get_records", label: "Get Records", kind: "query", help: "Return all transactions as JSON array.", sample: {} },

		// Transfers
		{ key: "record_transfer", label: "Record Transfer", kind: "mutation", help: "Transfer amount between wallets.", sample: { ...sampleTransaction, type: "TRANSFER", wallet_name: "Cash", related_wallet_name: "Bank", amount: 50 } },
		{ key: "update_transfer", label: "Update Transfer", kind: "mutation", help: "Update transfer record by id.", sample: { id: 1, type: "TRANSFER", wallet_name: "Cash", related_wallet_name: "Bank", amount: 60 } }
	];

	let inputs = $state<Record<string, string>>({});
	let results = $state<Record<string, { raw: string; parsed?: any; elapsed_ms: number }>>({});
	let consoleLogs = $state<{ ts: string; action: string; input: any; output?: any; error?: string; elapsed_ms?: number }[]>([]);

	// Prefill
	for (const op of operations) inputs[op.key] = JSON.stringify(op.sample, null, 2);

	let transport = $state<'auto' | 'window' | 'http'>('auto');
	let endpointBase = $state<string>('/api');

	function nowISO() { return new Date().toISOString(); }
	function log(action: string, input: any, output?: any, error?: string, elapsed_ms?: number) {
		consoleLogs.unshift({ ts: nowISO(), action, input, output, error, elapsed_ms });
		if (consoleLogs.length > 200) consoleLogs.pop();
	}

	function pretty(opKey: string) {
		try { inputs[opKey] = JSON.stringify(JSON.parse(inputs[opKey] ?? '{}'), null, 2); } catch {}
	}

	async function callBackend(fn: string, payloadText: string) {
		const body = payloadText?.trim() ? payloadText : '{}';
		const g: any = globalThis as any;

		const resolveWindowInvoker = () => {
			if (g.saucer?.exposed && typeof g.saucer.exposed[fn] === 'function')
				return (s: string) => g.saucer.exposed[fn](s);
			return null;
		};

		if (transport === 'window' || transport === 'auto') {
			const inv = resolveWindowInvoker();
			if (inv) return await inv(body);
		}

		if (transport === 'http' || transport === 'auto') {
			const res = await fetch(`${endpointBase}/${fn}`, {
				method: 'POST',
				headers: { 'Content-Type': 'application/json' },
				body
			});
			const text = await res.text();
			if (!res.ok) throw new Error(`${res.status} ${res.statusText}: ${text}`);
			return text;
		}

		throw new Error('No available transport. Expose via window.saucer.exposed or add HTTP endpoints.');
	}

	function parseMaybeJSON(text: string) {
		try { return JSON.parse(text); } catch { return undefined; }
	}

	async function run(op: Operation) {
		const payload = inputs[op.key] ?? '{}';
		const t0 = performance.now();
		try {
			const raw = await callBackend(op.key, payload);
			const elapsed_ms = Math.round(performance.now() - t0);
			const parsed = typeof raw === 'string' ? parseMaybeJSON(raw) : raw;
			results[op.key] = { raw: String(raw), parsed, elapsed_ms };
			log(`RUN ${op.key}`, payload, parsed ?? raw, undefined, elapsed_ms);
		} catch (err: any) {
			const elapsed_ms = Math.round(performance.now() - t0);
			results[op.key] = { raw: String(err?.message ?? err), parsed: undefined, elapsed_ms };
			log(`RUN ${op.key}`, payload, undefined, String(err?.message ?? err), elapsed_ms);
		}
	}

	function reset(op: Operation) {
		inputs[op.key] = JSON.stringify(op.sample, null, 2);
		results[op.key] = undefined as any;
	}
</script>

<div class="min-h-screen bg-slate-950 text-slate-100">
	<header class="sticky top-0 z-20 border-b border-slate-800 bg-slate-900/70 backdrop-blur">
		<div class="mx-auto max-w-7xl px-4 py-3 flex items-center justify-between">
			<h1 class="text-xl md:text-2xl font-semibold">MyWallet – Function Tester</h1>
			<div class="flex items-center gap-3 text-sm">
				<label class="opacity-80">Transport</label>
				<select bind:value={transport} class="bg-slate-800 rounded px-3 py-1.5 outline-none">
					<option value="auto">auto</option>
					<option value="window">window</option>
					<option value="http">http</option>
				</select>
				{#if transport !== 'window'}
					<input bind:value={endpointBase} class="bg-slate-800 rounded px-3 py-1.5 outline-none w-44" placeholder="/api" />
				{/if}
			</div>
		</div>
	</header>

	<main class="mx-auto max-w-7xl px-4 py-6 space-y-8">
		{#each ['query','mutation'] as groupKind}
			<section class="space-y-3">
				<h2 class="text-lg font-semibold capitalize">{groupKind} functions</h2>
				<div class="grid grid-cols-1 md:grid-cols-2 gap-4">
					{#each operations.filter(o => o.kind === groupKind) as op}
						<article class="rounded-2xl border border-slate-800 bg-slate-900/50">
							<div class="p-4 border-b border-slate-800 flex items-center justify-between">
								<div>
									<h3 class="font-semibold">{op.label}</h3>
									<p class="text-xs text-slate-400">{op.key}</p>
								</div>
								<div class="flex gap-2">
									<button class="rounded-xl bg-slate-800 px-3 py-1.5 text-sm hover:bg-slate-700" onclick={() => pretty(op.key)}>Pretty</button>
									<button class="rounded-xl bg-slate-800 px-3 py-1.5 text-sm hover:bg-slate-700" onclick={() => reset(op)}>Reset</button>
									<button class="rounded-xl bg-indigo-600 px-3 py-1.5 text-sm hover:bg-indigo-500" onclick={() => run(op)}>Run</button>
								</div>
							</div>
							<div class="p-4 space-y-3">
								<p class="text-sm text-slate-300">{op.help}</p>
								<textarea class="w-full h-40 rounded-xl bg-slate-950/70 outline-none p-3 text-slate-100 border border-slate-800 font-mono text-sm" bind:value={inputs[op.key]} spellcheck="false" />
								{#if results[op.key]}
									<div class="rounded-xl border border-slate-800 bg-slate-950/50">
										<div class="p-3 border-b border-slate-800 flex items-center justify-between">
											<strong class="text-sm">Result</strong>
											<span class="text-xs text-slate-400">{results[op.key].elapsed_ms} ms</span>
										</div>
										<div class="p-3 grid gap-3">
											<div>
												<div class="text-xs text-slate-400 mb-1">Parsed (if JSON)</div>
												<pre class="text-xs overflow-auto max-h-48 whitespace-pre-wrap">{JSON.stringify(results[op.key].parsed, null, 2) ?? '—'}</pre>
											</div>
											<div>
												<div class="text-xs text-slate-400 mb-1">Raw</div>
												<pre class="text-xs overflow-auto max-h-48 whitespace-pre-wrap">{results[op.key].raw}</pre>
											</div>
										</div>
									</div>
								{/if}
							</div>
						</article>
					{/each}
				</div>
			</section>
		{/each}

		<section class="space-y-3">
			<h2 class="text-lg font-semibold">Console</h2>
			<div class="rounded-2xl border border-slate-800 bg-slate-950/60">
				<div class="p-2 text-xs text-slate-400 border-b border-slate-800">Last {Math.min(consoleLogs.length, 200)} calls</div>
				<ul class="divide-y divide-slate-800 max-h-72 overflow-auto">
					{#each consoleLogs as logitem}
						<li class="p-3 text-xs">
							<div class="flex items-center justify-between">
								<div class="font-semibold">{logitem.action}</div>
								<div class="text-slate-400">{logitem.ts}</div>
							</div>
							{#if logitem.elapsed_ms != null}
								<div class="text-slate-400 mb-1">{logitem.elapsed_ms} ms</div>
							{/if}
							<div class="grid grid-cols-1 md:grid-cols-3 gap-2">
								<div><div class="text-slate-400">input</div><pre class="whitespace-pre-wrap break-all">{typeof logitem.input === 'string' ? logitem.input : JSON.stringify(logitem.input, null, 2)}</pre></div>
								<div><div class="text-slate-400">output</div><pre class="whitespace-pre-wrap break-all">{typeof logitem.output === 'string' ? logitem.output : JSON.stringify(logitem.output, null, 2)}</pre></div>
								<div><div class="text-slate-400">error</div><pre class="whitespace-pre-wrap break-all">{logitem.error ?? '—'}</pre></div>
							</div>
						</li>
					{/each}
				</ul>
			</div>
		</section>
	</main>
</div>

<style>
	pre, textarea { scrollbar-color: #475569 transparent; }
</style>
