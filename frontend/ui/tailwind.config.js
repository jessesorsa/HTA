export default {
  content: ["./src/**/*.{js,svelte}"],
  plugins: [require("@tailwindcss/typography"), require("daisyui")],
  theme: {
    extend: {
      colors: {
        'light-grey-color': '#F6F6F6', // Replace '#ff0000' with your desired custom color code
      },
    },
  },
};