#pragma once

#include <cstddef>
#include <deque>

/**
 * @brief A simple library class for estimating remaining range in an electric vehicle.
 * 
 * The estimator uses a rolling average of consumption data (kWh/km) to compute
 * how many kilometers remain given the current battery state of charge.
 */
class RangeEstimator {
public:
    /**
     * @param initSoC     Initial battery state of charge [0..1].
     * @param windowSize  Number of recent data points to use for rolling average.
     */
    RangeEstimator(double initSoC = 1.0, size_t windowSize = 10);

    /**
     * @brief Update the battery's State of Charge (SoC).
     * @param newSoC  New state of charge [0..1].
     */
    void setStateOfCharge(double newSoC);

    /**
     * @brief Add a new driving segment to the rolling consumption data.
     * 
     * @param distanceKm      Distance traveled in this segment (km).
     * @param energyUsedKWh   Energy used (kWh) during this segment.
     */
    void addDrivingData(double distanceKm, double energyUsedKWh);

    /**
     * @brief Compute the remaining range based on current SoC and average consumption.
     * @return Estimated remaining range in km.
     */
    double getEstimatedRangeKm() const;

private:
    double m_currentSoC;                ///< Current battery SoC [0..1].
    size_t m_windowSize;                ///< Number of consumption data points to keep.
    std::deque<double> m_consumptionHistory;  ///< Rolling window of consumption (kWh/km).

    /**
     * A fallback factor (km/kWh) if no driving data is available.
     * Example: 5 km/kWh as a default.
     */
    static constexpr double kDefaultRangeFactor = 5.0;
};
