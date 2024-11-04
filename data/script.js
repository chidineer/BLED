document.addEventListener('DOMContentLoaded', function() {
    const onButton = document.getElementById('onButton');
    const offButton = document.getElementById('offButton');
    const ledIndicator = document.getElementById('ledIndicator');
    const statusText = document.getElementById('statusText');
    
    let isConnected = true;
    
    function updateLedStatus(isOn) {
        ledIndicator.classList.toggle('on', isOn);
        statusText.textContent = `LED is ${isOn ? 'ON' : 'OFF'}`;
    }
    
    function showLoading(button) {
        const originalText = button.innerHTML;
        button.innerHTML = '<span class="button-icon">⌛</span>Loading...';
        button.disabled = true;
        return () => {
            button.innerHTML = originalText;
            button.disabled = false;
        };
    }
    
    async function handleButtonClick(endpoint, button) {
        if (!isConnected) return;
        
        const resetButton = showLoading(button);
        
        try {
            const response = await fetch(endpoint);
            const data = await response.json();
            updateLedStatus(data.status === 'on');
        } catch (error) {
            console.error('Error:', error);
            statusText.textContent = 'Error occurred!';
        } finally {
            resetButton();
        }
    }
    
    onButton.addEventListener('click', () => handleButtonClick('/LED_on', onButton));
    offButton.addEventListener('click', () => handleButtonClick('/LED_off', offButton));
    
    // Add touch feedback for mobile devices
    const buttons = document.querySelectorAll('.control-button');
    buttons.forEach(button => {
        button.addEventListener('touchstart', function() {
            this.style.transform = 'scale(0.95)';
        });
        button.addEventListener('touchend', function() {
            this.style.transform = 'scale(1)';
        });
    });
});