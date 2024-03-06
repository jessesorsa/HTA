<script>
    import "../app.css";
    import Footer from "../components/Footer.svelte";
    import Navbar from "../components/Navbar.svelte";
    import SideBar from "../components/SideBar.svelte";
</script>

<Navbar />
<div class="flex min-h-screen">
    <SideBar />
    <div class="flex-grow min-h-full">
        <slot />
    </div>
</div>
<Footer />
