<!--In this file, we are creating the base layout for our application
It will be displayed across all the pages of our application.
This includes

-->

<script>
	import '../app.css';
	import appIcon from '$lib/assets/icon.png';

	let { children } = $props();
</script>

<svelte:head>
	<link rel="icon" href={appIcon} />
</svelte:head>

{@render children?.()}
