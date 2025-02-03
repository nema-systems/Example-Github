#include "RangeEstimator.h"
#include <algorithm>    // for std::accumulate, std::max, std::min
#include <numeric>      // for std::accumulate

// ------------------------------------------------------------
// Hardcoded battery capacity for this particular vehicle (in kWh).
// ------------------------------------------------------------
// @nema:get:https://app.nemasystems.io/nema-public-demo/default/demo-ev-performance/data/7
static constexpr double VEHICLE_BATTERY_CAPACITY_KWH = 60.0;

// ------------------------------------------------------------
// Constructor
// ------------------------------------------------------------
RangeEstimator::RangeEstimator(double initSoC, size_t windowSize)
    : m_currentSoC(initSoC),
      m_windowSize(windowSize)
{
    // Clamp the initial SoC to [0..1]
    if (m_currentSoC < 0.0) m_currentSoC = 0.0;
    if (m_currentSoC > 1.0) m_currentSoC = 1.0;
}

// ------------------------------------------------------------
// Set the state of charge (SoC)
// ------------------------------------------------------------
void RangeEstimator::setStateOfCharge(double newSoC) {
    if (newSoC < 0.0) newSoC = 0.0;
    if (newSoC > 1.0) newSoC = 1.0;
    m_currentSoC = newSoC;
}

// ------------------------------------------------------------
// Add new driving data to the rolling window
// ------------------------------------------------------------
void RangeEstimator::addDrivingData(double distanceKm, double energyUsedKWh) {
    if (distanceKm <= 0.0) {
        // Invalid data; ignore
        return;
    }
    double consumptionKWhPerKm = energyUsedKWh / distanceKm;

    // Push consumption into the queue; pop old data if we exceed window size
    m_consumptionHistory.push_back(consumptionKWhPerKm);
    if (m_consumptionHistory.size() > m_windowSize) {
        m_consumptionHistory.pop_front();
    }
}

// ------------------------------------------------------------
// Compute the estimated remaining range
// ------------------------------------------------------------
double RangeEstimator::getEstimatedRangeKm() const {
    // If no consumption data is available, fallback to a default range factor
    if (m_consumptionHistory.empty()) {
        return m_currentSoC * VEHICLE_BATTERY_CAPACITY_KWH * kDefaultRangeFactor;
    }

    // Calculate the rolling average consumption (kWh/km)
    double sum = 0.0;
    for (double c : m_consumptionHistory) {
        sum += c;
    }
    double avgConsumptionKWhPerKm = sum / m_consumptionHistory.size();

    // Sanity check; avoid division by zero or negative consumption
    if (avgConsumptionKWhPerKm <= 0.0) {
        return m_currentSoC * VEHICLE_BATTERY_CAPACITY_KWH * kDefaultRangeFactor;
    }

    // Remaining usable energy in the battery
    double usableEnergyKWh = m_currentSoC * VEHICLE_BATTERY_CAPACITY_KWH;

    // Estimated range = (usable energy) / (average consumption)
    //                 = kWh / (kWh/km) = km
    double estimatedRangeKm = usableEnergyKWh / avgConsumptionKWhPerKm;
    return estimatedRangeKm;
}
