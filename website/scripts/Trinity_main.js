let dist = document.getElementById("dist");
let dist_inches = document.getElementById("dist-in");
let gas = document.getElementById("gas");
let gas_box = document.getElementById("gas-box");
let temp = document.getElementById("temp");
let temp_f = document.getElementById("temp-f");

// Check to see when data updates on the website
const observer = new MutationObserver((mutations) => {
    for (mutation of mutations) {
        if (mutation.type === "childList") {
            // Change the background of the gas data box
            if (gas.textContent >= 750) {
                gas_box.style.backgroundColor = "lightcoral";
            }
            else if (gas.textContent >= 250) {
                gas_box.style.backgroundColor = "orange";
            }
            else {
                gas_box.style.backgroundColor = "lightgreen";
            }

            // Convert distance from centimeters to inches rounded to 2 decimal places
            dist_inches.textContent = Math.round(dist.textContent / 0.0254) / 100;

            // Convert temperature from Celsius to fahrenheit rounded to 1 decimal place
            temp_f.textContent = Math.round(temp.textContent * 90 / 5) / 10 + 32;
        }
    }
})

observer.observe(gas, {attributes: true, characterData: true, subtree: true, childList: true});