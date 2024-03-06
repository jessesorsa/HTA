<script>
    import SessionHead from "./SessionHead.svelte";
    import SessionBody from "./SessionBody.svelte";
</script>

<div class="rounded-3xl">
    <table class="table bg-base-100 shadow w-full">
        <thead>
            <SessionHead />
        </thead>
        <tbody>
            <SessionBody />
        </tbody>
    </table>
</div>
